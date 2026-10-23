/*
 * A153841  First Sunday in n-th month of 365-day year starting on Tuesday.
 * https://oeis.org/A153841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = ((7 - ((2 + ((n-1)/12)%7 + offset[(n-1)%12])%7))%7) + 1
 *            where offset[k] is the cumulative sum of days of months 1..k
 *            modulo 7. The year cycles every 12 months, and each year advance
 *            the weekday by 1
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cumulative month-lengths mod 7 for the 12 months of a 365‑day year.
       offset[i] = (sum_{m=1}^{i} month_len(m)) mod 7, with offset[0] = 0. */
    static const unsigned char offset[12] = {
        0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5
    };

    idx_t year   = (n - 1) / 12;          /* number of complete years before month n */
    idx_t month  = (n - 1) % 12;          /* month index within the current year (0‑based) */
    seq_t day_of_week_start =
        (seq_t)(2 + (year % 7) + offset[(size_t)month]); /* 0=Sunday */

    day_of_week_start %= 7;
    seq_t first_sunday = ((7 - day_of_week_start) % 7) + 1;
    return first_sunday;                    /* 1‑based day number in the month */
}
