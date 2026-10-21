/*
 * A210889  Number of days after Mar 01 00 such that the date written in the
 *          format D.M.YY (and considered as a number without the dots) is
 *          palindromic.
 * https://oeis.org/A210889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 322, u32 n <= 322, u64 n <= 322, u128 n <= 322,
 *            big any n
 * Helpers:   is_palindrome_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n), the number of days after March 1 00 to the n‑th
   date (between 2000‑03‑01 and 2099‑12‑31) whose D.M.YY representation,
   written as a decimal number with no leading zeros on D and M but always
   two digits for YY, is a palindrome.  The sequence contains exactly 322
   terms.  The function precomputes all palindromic dates in the interval
   and then returns the stored value. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_palindrome_num.h"

static inline bool is_leap_year(idx_t y)
{
    return ((y % 400 == 0) || ((y % 4 == 0) && (y % 100 != 0)));
}

static inline idx_t days_in_month(idx_t y, idx_t m)
{
    switch (m) {
        case 1: return 31;
        case 2: return is_leap_year(y) ? 29 : 28;
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
        default: return 0;
    }
}

static bool _pals_inited = false;
static seq_t _pal_diff[400];
static idx_t _pal_cnt = 0;

static void _init_palindromes(void)
{
    if (_pals_inited)
        return;

    const idx_t start_year = 2000;
    const idx_t end_year   = 2099;

    seq_t year_cum[101];           /* cumulative days up to the start of each year */
    year_cum[0] = 0;
    for (idx_t i = 0; i < end_year - start_year; ++i) {
        seq_t days = is_leap_year(start_year + i) ? 366 : 365;
        year_cum[i + 1] = year_cum[i] + days;
    }

    /* reference day: March 1 2000 */
    seq_t ref_day = year_cum[0];
    for (idx_t m = 1; m <= 2; ++m)
        ref_day += days_in_month(start_year, m);
    /* March 1 gives day-1 == 0 */

    for (idx_t year = start_year; year <= end_year; ++year) {
        idx_t month_start = (year == start_year) ? 3 : 1;
        seq_t cum_year = year_cum[year - start_year];
        for (idx_t m = month_start; m <= 12; ++m) {
            idx_t month_len = days_in_month(year, m);
            for (idx_t d = 1; d <= month_len; ++d) {
                int yy = (int)(year % 100);
                int dd = (int)d;
                int mm = (int)m;

                int num;
                if (mm <= 9)
                    num = dd * 1000 + mm * 100 + yy;
                else
                    num = dd * 10000 + mm * 100 + yy;

                if (is_palindrome_num(num)) {
                    seq_t cum = cum_year;
                    for (idx_t k = 1; k < m; ++k)
                        cum += days_in_month(year, k);
                    cum += (seq_t)(d - 1);
                    seq_t diff = cum - ref_day;
                    if (_pal_cnt < 400)
                        _pal_diff[_pal_cnt++] = diff;
                }
            }
        }
    }
    _pals_inited = true;
}

seq_t a(idx_t n)
{
    _init_palindromes();
    if (n == 0 || n - 1 >= _pal_cnt)
        return 0;
    return _pal_diff[n - 1];
}
