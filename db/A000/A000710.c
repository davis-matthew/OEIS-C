/*
 * A000710  Number of partitions of n, with two kinds of 1, 2, 3 and 4.
 * https://oeis.org/A000710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: 1/((1-x)(1-x^2)(1-x^3)(1-x^4)*Product_{k>=1} (1-x^k));
 *            partitions p(m) from Euler's pentagonal recurrence, then divided
 *            by (1-x^k) for k=1..4
 * Range:     u8 n <= 8, u16 n <= 25, u32 n <= 82, u64 n <= 300,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"

seq_t a(idx_t n) {
    idx_t N = n + 1;
    seq_t *p = alloc_zero(N);
    p[0] = 1;
    for (idx_t m = 1; m <= n; m++) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            if (k & 1) pos += p[m - g1]; else neg += p[m - g1];
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= m) {
                if (k & 1) pos += p[m - g2]; else neg += p[m - g2];
            }
        }
        p[m] = pos - neg;
    }
    for (idx_t k = 1; k <= 4; k++) {
        for (idx_t m = k; m <= n; m++) {
            p[m] += p[m - k];
        }
    }
    seq_t result = p[n];
    free(p);
    return result;
}
