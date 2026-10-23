/*
 * A162240  Years in which January first falls on a Wednesday (since 2001).
 * https://oeis.org/A162240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth year (>=2001) in which January 1 falls on a
 *            Wednesday
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t year = 2001;          /* next year to test */
    int   weekday = 1;          /* 0=Sunday, 1=Monday, … 6=Saturday. 2001-01-01 is Monday. */
    idx_t count = 0;            /* how many qualifying years we have found */

    while (1)
    {
        if (weekday == 3)       /* Wednesday */
        {
            ++count;
            if (count == n)
                return (seq_t)year;    /* found the nth year */
        }

        /* advance to the next year */
        bool leap = (year % 4 == 0 &&
                     (year % 100 != 0 || year % 400 == 0));
        weekday = (weekday + (leap ? 2 : 1)) % 7;
        ++year;
    }
}
