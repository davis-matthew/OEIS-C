/*
 * A224941  Non-leap years beginning and ending on Wednesday.
 * https://oeis.org/A224941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th non‑leap year whose January 1 is a Wednesday * (the
 *            year therefore ends on a Wednesday because it has 365 days)
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   weekday_jan1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/weekday_jan1.h"

/* Gregorian leap‑year test. */
static inline bool is_leap_year(int year)
{
    if (year % 400 == 0)
        return true;
    if (year % 100 == 0)
        return false;
    return (year % 4 == 0);
}

/* True iff year is a non‑leap year that starts on Wednesday. */
static inline bool nonleap_and_wednesday(int year)
{
    return !is_leap_year(year) && (weekday_jan1(year) == 4); /* Wednesday */
}

/* OEIS sequence A224941 */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. If n = 0, return 0 as a safe
       placeholder; such an index never occurs in the OEIS data set. */
    if (n == (idx_t)0)
        return (seq_t)0;

    idx_t count = (idx_t)1;   /* number of suitable years found so far */
    int year  = 1583;         /* start after the Gregorian reform */

    for (;;)
    {
        if (nonleap_and_wednesday(year))
        {
            if (count == n)
                return (seq_t)year;
            ++count;
        }
        ++year;
    }
}
