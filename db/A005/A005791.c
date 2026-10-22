/*
 * A005791  5-dimensional Catalan numbers.
 * https://oeis.org/A005791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(5n-4)*(5n-3)*(5n-2)*(5n-1)*a(n-1) /
 *            ((n+1)*(n+2)*(n+3)*(n+4)), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   g64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/g64.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    for (idx_t i = 1; i <= n; i++) {
        uint64_t num[5], den[4];
        int p, q;
        num[0] = 5;
        num[1] = 5 * i - 4;
        num[2] = 5 * i - 3;
        num[3] = 5 * i - 2;
        num[4] = 5 * i - 1;
        den[0] = i + 1;
        den[1] = i + 2;
        den[2] = i + 3;
        den[3] = i + 4;
        /* cancel common factors so remaining numerator and denominator are coprime */
        for (q = 0; q < 4; q++)
            for (p = 0; p < 5; p++) {
                uint64_t g = g64(num[p], den[q]);
                if (g > 1) { num[p] /= g; den[q] /= g; }
            }
        /* the remaining denominator now divides r exactly */
        for (q = 0; q < 4; q++)
            if (den[q] > 1) r /= (seq_t)den[q];
        for (p = 0; p < 5; p++)
            if (num[p] > 1) r *= (seq_t)num[p];
    }
    return r;
}
