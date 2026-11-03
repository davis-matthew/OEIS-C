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
 * Validated: standard, pass, 55 terms through n = 296 (2026-11-01)
 * Helpers:   none
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

/* Number of days from 01‑Jan‑0000 to the given Gregorian date.
 * All calculations are performed in int64_t and then cast to seq_t. */
static inline seq_t days_from_civil(int year, unsigned month, unsigned day)
{
    int y = year;
    unsigned m = month;
    if (m <= 2) { y -= 1; m += 12; }

    int64_t era = (int64_t)((y >= 0 ? y : y-399) / 400);
    int64_t yoe = (int64_t)y - era * 400;
    int64_t doy = (int64_t)((153 * (m-3) + 2) / 5 + (int64_t)day - 1);
    int64_t doe = yoe * 365 + yoe/4 - yoe/100 + yoe/400 + doy;
    int64_t days = era * 146097 + doe;
    return (seq_t)days;
}

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
