/*
 * A116386  Number of calendar weeks in the year n (starting at n=0 for the
 *          year 2000).
 * https://oeis.org/A116386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 54 if the year 2000 + n is a leap year that begins on
 *            Saturday; otherwise a(n) = 53
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: determine if a Gregorian year is a leap year. */
static inline bool is_leap_year(idx_t year)
{
    return ( (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0) );
}

seq_t a(idx_t n)
{
    /* 2000-01-01 was a Saturday.  Represent Sunday as 0. */
    const idx_t weekday_sat = 6;          /* Saturday */
    const idx_t base_year   = 2000;

    /* Year corresponding to index n. */
    idx_t Y = n + base_year;

    /* Count of leap years from year 0 up to Y-1.  The number of
       leap years <= 1999 is a constant (484). */
    idx_t y_minus_1 = Y - 1;
    idx_t leaps = y_minus_1 / 4 - y_minus_1 / 100 + y_minus_1 / 400
                  - (idx_t)484;

    /* The shift from the base weekday due to the years elapsed. */
    idx_t days_mod7 = (n % 7 + leaps % 7) % 7;
    idx_t weekday_jan1 = (weekday_sat + days_mod7) % 7;

    bool leap = is_leap_year(Y);

    return (seq_t)( (leap && weekday_jan1 == weekday_sat) ? 54 : 53 );
}
