/*
 * A210885  Number of days after Jan 01 1000 such that the date written in the
 *          format DDMMYYYY is palindromic.
 * https://oeis.org/A210885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 55, u32 n <= 335, u64 n <= 335, u128 n <= 335,
 *            big any n
 * Helpers:   days_from_civil
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Palindromic dates sequence A210885:
 * a(n) = number of days after 01‑Jan‑1000 until the n‑th DDMMYYYY
 *  palindrome (Gregorian calendar applied for all years). Computed by
 *  brute‑force scan of all dates from 1000‑01‑01 to 9999‑12‑31. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/days_from_civil.h"

/* Return true if y is a Gregorian leap year. */
static inline bool is_leap_year(int y)
{
    return ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0);
}

/* Days in month m of year y (Gregorian). */
static inline unsigned month_days(int y, unsigned m)
{
    static const unsigned md[12] =
        {31,28,31,30,31,30,31,31,30,31,30,31};
    if (m == 2)
        return 28 + (unsigned)is_leap_year(y);
    return md[m-1];
}

/* Pre‑computed table of day offsets for the 335 palindromic dates. */
static bool table_built = false;
static seq_t tbl[335];
static size_t tbl_len = 0;

static void build_tbl(void)
{
    if (table_built) return;
    const seq_t base = days_from_civil(1000, 1, 1);
    tbl_len = 0;
    char dig[8];
    for (int y = 1000; y <= 9999; ++y) {
        for (unsigned m = 1; m <= 12; ++m) {
            unsigned dm = month_days(y, m);
            for (unsigned d = 1; d <= dm; ++d) {
                dig[0] = '0' + (d/10);
                dig[1] = '0' + (d%10);
                dig[2] = '0' + (m/10);
                dig[3] = '0' + (m%10);
                dig[4] = '0' + (y/1000);
                dig[5] = '0' + ((y/100)%10);
                dig[6] = '0' + ((y/10)%10);
                dig[7] = '0' + (y%10);
                if (dig[0]==dig[7] && dig[1]==dig[6] &&
                    dig[2]==dig[5] && dig[3]==dig[4]) {
                    tbl[tbl_len++] = days_from_civil(y, m, d) - base;
                }
            }
        }
    }
    table_built = true;
}

/* External function required by the task. */
seq_t a(idx_t n)
{
    if (!table_built) build_tbl();
    if (n == 0) return 0;
    if (n > tbl_len) return 0;          /* outside defined sequence */
    return tbl[(size_t)n - 1];
}
