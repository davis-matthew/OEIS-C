/*
 * A006013  a(n) = binomial(3*n+1,n)/(n+1).
 * https://oeis.org/A006013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = binomial(3*n+1,n)/(n+1), computed via a(n) = a(n-1) *
 *            3*(3n+1)*(3n-1) / (2*(n+1)*(2n+1)), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        uint64_t num[3], den[3];
        int i, j;
        num[0] = 3; num[1] = 3 * (uint64_t)k + 1; num[2] = 3 * (uint64_t)k - 1;
        den[0] = 2; den[1] = (uint64_t)k + 1; den[2] = 2 * (uint64_t)k + 1;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3 && den[i] > 1; j++) {
                uint64_t g = gcd_u64(num[j], den[i]);
                num[j] /= g;
                den[i] /= g;
            }
            if (den[i] > 1) {
                uint64_t rem = (uint64_t)(r % (seq_t)den[i]);
                uint64_t g = gcd_u64(rem, den[i]);
                r /= (seq_t)g;
                den[i] /= g;
            }
        }
        for (j = 0; j < 3; j++) r *= (seq_t)num[j];
        for (i = 0; i < 3; i++) if (den[i] > 1) r /= (seq_t)den[i];
    }
    return r;
}
