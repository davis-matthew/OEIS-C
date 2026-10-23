/*
 * A188528  First month in year n with a "Friday the 13th", starting from
 *          1901.
 * https://oeis.org/A188528
 *
 * Offset:    1901    (a(1901) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3000, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  First month in year n that has a Friday the 13th.
 *  A short precomputation of a 400‑year cycle is used, because the
 *  Gregorian calendar repeats every 400 years.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 400‑year cycle table, initialized once. */
    static seq_t table[400];
    static bool init = false;

    if (!init) {
        /* Precompute for the years 1901 … 2300 (400 years). */
        for (int y = 1901; y <= 1901 + 399; ++y) {
            for (int m = 1; m <= 12; ++m) {
                /* Zeller’s congruence for Gregorian calendar. */
                int q  = 13;           /* day of month */
                int mm = m;
                int yy = y;

                if (mm <= 2) {        /* January, February treated as months 13, 14 of previous year */
                    mm += 12;
                    yy -= 1;
                }

                int K = yy % 100;      /* year of the century */
                int J = yy / 100;      /* zero‑based century */

                /* h: 0=Saturday, 1=Sunday, …, 6=Friday */
                int h = (q + (13 * (mm + 1)) / 5 + K + K / 4 + J / 4 + 5 * J) % 7;

                if (h == 6) {         /* Friday */
                    table[y - 1901] = (seq_t)m;
                    break;
                }
            }
        }
        init = true;
    }

    if (n < 1901)
        return 0;                 /* undefined for years before 1901 */

    idx_t offset = (n - 1901) % 400;  /* periodicity of 400 years */
    return table[offset];
}
