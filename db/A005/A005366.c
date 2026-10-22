/*
 * A005366  Hoggatt sequence with parameter d=8.
 * https://oeis.org/A005366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} t(k), where t(0) = 1 and t(k+1) = t(k) *
 *            Prod_{i=0..7} (n+i-k) / ((k+1) * Prod_{j=2..8} (j+k)), i.e. the
 *            terms of Hypergeometric8F7([-7-n,...,-n],[2,...,8],1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   a005366_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005366_gcd.h"

seq_t a(idx_t n) {
    seq_t t = 1;
    seq_t s = 1;
    for (idx_t k = 0; k < n; k++) {
        uint64_t num[8], den[8];
        int i, j;
        for (i = 0; i < 8; i++)
            num[i] = (uint64_t)(n + (idx_t)i - k);
        den[0] = (uint64_t)(k + 1);
        for (j = 2; j <= 8; j++)
            den[j - 1] = (uint64_t)(k + (idx_t)j);

        /* cancel each denominator factor against the numerator factors and t */
        for (j = 0; j < 8; j++) {
            uint64_t d = den[j];
            for (i = 0; i < 8 && d > 1; i++) {
                uint64_t g = a005366_gcd(d, num[i]);
                num[i] /= g;
                d /= g;
            }
            if (d > 1) {
                uint64_t r = (uint64_t)(t % d);
                uint64_t g = (r == 0) ? d : a005366_gcd(d, r);
                t /= g;
                d /= g;
            }
        }
        for (i = 0; i < 8; i++)
            t *= num[i];
        s += t;
    }
    return s;
}
