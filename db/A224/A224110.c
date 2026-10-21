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
 * Helpers:   easter_calc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/easter_calc.h"

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
