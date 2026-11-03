/*
 * A224110  Easter occurrences on March 22, March 23, ..., April 25 during a
 *          5,700,000-year Gregorian Easter cycle.
 * https://oeis.org/A224110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of Easter Sundays occurring on the n-th calendar
 *            day from March 22 to April 25 in the 5 700 000‑year Gregorian
 *            cycle
 * Range:     u8 none, u16 n <= 2, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: full, pass, 35 terms through n = 35 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute Easter (month, day) for year y using the
   Meeus/Jones/Butcher algorithm.  month: 3=March, 4=April.
   day: 1-31. */
static void easter_calc(idx_t y, int *month, int *day)
{
    int a = y % 19;
    int b = y / 100;
    int c = y % 100;
    int d = b / 4;
    int e = b % 4;
    int f = (b + 8) / 25;
    int g = (b - f + 1) / 3;
    int h = (19 * a + b - d - g + 15) % 30;
    int i = c / 4;
    int k = c % 4;
    int l = (32 + 2 * e + 2 * i - h - k) % 7;
    int m = (a + 11 * h + 22 * l) / 451;
    int mm = (h + l - 7 * m + 114) / 31;      /* 3 or 4 */
    int dd = ((h + l - 7 * m + 114) % 31) + 1;
    *month = mm;
    *day   = dd;
}

seq_t a(idx_t n)
{
    /* The sequence has 35 terms.  For n outside 1..35 we return 0. */
    if (n < 1 || n > 35)
        return (seq_t)0;

    /* static cache: compute once, reuse for all calls. */
    static seq_t counts[35] = {0};
    static int initialized = 0;

    if (!initialized) {
        /* Count Easter dates for years 1 .. 5 700 000. */
        const idx_t MAX_YEAR = 5700000;
        int month, day;
        for (idx_t year = 1; year <= MAX_YEAR; ++year)
        {
            easter_calc(year, &month, &day);
            int idx;
            if (month == 3)           /* March 22 .. 31  -> indices 0..9   */
                idx = day - 22;
            else                      /* April 1 .. 25   -> indices 10..34 */
                idx = 10 + (day - 1);
            counts[idx] += 1;
        }
        initialized = 1;
    }

    return counts[n - 1];
}
