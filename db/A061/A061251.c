/*
 * A061251  Days in year to end of n-th month starting Jan 01 in a year just
 *          following a leap year or millennium.
 * https://oeis.org/A061251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = cumulative days from Jan 01 2001 to end of the n‑th month
 *            (months counted from 0). The year is treated as a Gregorian
 *            year, so February has 29 days in leap years (divisible by 4 but
 *            not by 100 unless divisible by 400). The sequence starts in the
 *            year just after a leap year (2001 in the OEI...
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const unsigned month_len_normal[12] =
{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static const unsigned month_len_leap[12] =
{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t years_passed = n / 12;
    idx_t months_in_cur  = n % 12;
    idx_t year = 2001;

    /* Add full years before the current year */
    for (idx_t i = 0; i < years_passed; ++i) {
        bool leap = ( (year % 400 == 0) ||
                      ( (year % 4 == 0) && (year % 100 != 0) ) );
        total += (seq_t)(leap ? 366 : 365);
        ++year;
    }

    /* Add months in the current year */
    if (months_in_cur > 0) {
        bool leap = ( (year % 400 == 0) ||
                      ( (year % 4 == 0) && (year % 100 != 0) ) );
        const unsigned *tab = leap ? month_len_leap : month_len_normal;
        for (idx_t m = 0; m < months_in_cur; ++m) {
            total += (seq_t)tab[m];
        }
    }

    return total;
}
