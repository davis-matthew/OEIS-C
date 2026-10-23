/*
 * A162238  Years in which January first falls on a Monday (since 2001).
 * https://oeis.org/A162238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162238: Years in which January 1 falls on a Monday (since 2001).
   The function iterates through the years starting at 2001, keeps track of
   the weekday of January 1, and returns the n‑th year for which that weekday
   is Monday.  */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n >= 1; undefined for n < 1 – return 0 as a safe fallback. */
    if (n == 0) return (seq_t)0;

    idx_t year = 2001;          /* current year */
    uint8_t wd = 1;              /* weekday of Jan 1 in 2001: 1 = Monday */
    idx_t seen = 0;              /* how many Monday‑Jan‑1 years found */

    while (true)
    {
        if (wd == 1)            /* Jan 1 is Monday */
        {
            seen++;
            if (seen == n)
                return (seq_t)year;
        }

        /* Advance weekday to next year:
           365 days → +1 day, 366 days (leap) → +2 days. */
        bool leap = ((year % 4 == 0 && year % 100 != 0) ||
                     (year % 400 == 0));
        wd = (wd + (leap ? 2 : 1)) % 7;

        year++;                 /* next year */
    }
}
