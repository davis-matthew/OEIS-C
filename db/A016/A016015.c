/*
 * A016015  Inverse of 2006th cyclotomic polynomial.
 * https://oeis.org/A016015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power‐series 1/Φ2006(x) =
 *            ((x-1)(x^34-1)(x^118-1)(x^1003-1)) / ((1-x^2)(1-x^17)(1-x^59))
 *            Period: 2006
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t cache[2006];
    static bool initialized = false;

    if (!initialized) {
        /* initialise all entries to 0 */
        for (size_t i = 0; i < 2006; ++i) cache[i] = 0;

        const int ds[4] = {1, 34, 118, 1003};

        for (int mask = 0; mask < 16; ++mask) {
            int offset = 0;       /* exponent contributed by this subset */
            int popcnt = 0;       /* number of '-1' selections -> sign parity */

            for (int i = 0; i < 4; ++i) {
                if (mask & (1 << i)) {        /* choose the '-1' term */
                    ++popcnt;
                } else {                       /* choose the x^d term */
                    offset += ds[i];
                }
            }

            int sign = (popcnt & 1) ? -1 : 1;

            for (int m = 0; ; ++m) {
                int idx2 = offset + 2 * m;
                if (idx2 > 2005) break;
                for (int n1 = 0; ; ++n1) {
                    int idx3 = idx2 + 17 * n1;
                    if (idx3 > 2005) break;
                    for (int p = 0; ; ++p) {
                        int idx = idx3 + 59 * p;
                        if (idx > 2005) break;
                        cache[idx] += sign;
                    }
                }
            }
        }
        initialized = true;
    }

    size_t idx = (size_t)(n % 2006ull);
    return cache[idx];
}
