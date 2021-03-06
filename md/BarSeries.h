#ifndef BAR_SERIES_H
#define BAR_SERIES_H

#include "Bar.h"
#include "TSeries.h"
#include "Constant.h"
#include "Poco/NumberFormatter.h"
#include "Converter.h"

namespace Pheux { namespace Core {

    class BarSeries : public TSeries<Bar*>
    {
    public:
        BarSeries(Bar* proto, int length = 0) : proto(proto), length(length) {}
        ~BarSeries() {}
        friend class Indicator;

        void Calculate(Tick* tick)
        {
            if (size() <= 0)
            {
                Bar* bar(proto);
                bar->Init(tick);
                add(bar);
            }
            else
            {
                Bar* newbar = Latest()->Calculate(tick);
                if (newbar != NULL)
                    this->add(newbar);
            }
        }

        string CombinedType() const
        {
            return BarShortName[Type()] + Converter::ToString(Period());
        }

        Bar::BarType Type() const { return proto->Type(); }

        int Period() const { return proto->Period; }

        int Length() const { return (length <= 0 || length > size()) ? size() : length; }

        Bar* Latest() const { return this->operator[](0); }

        BarSeries* Clone()
        {
            BarSeries* series = new BarSeries(BarFactory::CloneBar(proto), length);
            for (int i = 0; i < size(); i++)
                series->_data.push_back(BarFactory::CloneBar(_data[i]));
            return series;
        }

    private:
        int length;    // mark the bar length which the indicator takes care
        Bar* proto; // prototype for the derived class
    };

}}

#endif
