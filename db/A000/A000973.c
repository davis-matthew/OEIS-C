/*
 * A000973  Fermat coefficients.
 * https://oeis.org/A000973
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-8, 7)/8
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 59, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a000973_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000973_gcd.h"

seq_t a(idx_t n) {
    uint64_t f[7];
    static const uint64_t den[7] = {2, 3, 4, 5, 6, 7, 8};
    uint64_t m = 2 * (uint64_t)n - 8;
    int i, j;
    seq_t r;

    for (i = 0; i < 7; i++)
        f[i] = m - (uint64_t)i;

    /* cancel the denominator 7! * 8 against the numerator factors */
    for (j = 0; j < 7; j++) {
        uint64_t d = den[j];
        while (d > 1) {
            for (i = 0; i < 7 && d > 1; i++) {
                uint64_t g = a000973_gcd(d, f[i]);
                if (g > 1) {
                    f[i] /= g;
                    d /= g;
                }
            }
        }
    }

    r = 1;
    for (i = 0; i < 7; i++)
        r *= (seq_t)f[i];
    return r;
}
