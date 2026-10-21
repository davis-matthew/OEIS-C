/*
 * A224950  Leap years having 53 Fridays and Saturdays.
 * https://oeis.org/A224950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   jan1_weekday
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate over years starting at 1583 and count leap years
 * whose January 1 is a Friday.  The Gregorian calendar repeats every
 * 400 years, so this produces the OEIS sequence A224950 for all
 * n ≥ 1. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/jan1_weekday.h"

static inline bool is_leap_year(uint64_t y)
{
    return ((y % 4ULL == 0ULL) && ((y % 100ULL != 0ULL) || (y % 400ULL == 0ULL)));
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t count  = 0;
    /* The first year satisfying the property is 1588.  Start
       our search after the Gregorian reform. */
    for (uint64_t year = 1583ULL; count < n; ++year) {
        if (!is_leap_year(year))
            continue;
        /* Friday is encoded as 5, see jan1_weekday(). */
        if (jan1_weekday(year) == 5) {
            ++count;
            result = (seq_t)year;
        }
    }
    return result;
}
