/*
 * A000837  Number of partitions of n into relatively prime parts. Also
 *          aperiodic partitions.
 * https://oeis.org/A000837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{d|n} mu(d)*A000041(n/d) for n >= 1, a(0) = 1;
 *            A000041 computed by Euler's pentagonal number recurrence
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 127, u64 n <= 416,
 *            u128 n <= 1458, big any n
 * Helpers:   mu_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mu_local.h"

seq_t a(idx_t n) {
    seq_t *p;
    seq_t pos, neg, result;
    idx_t m, k, d;

    if (n == 0) return 1;

    p = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!p) return 0;
    p[0] = 1;
    for (m = 1; m <= n; m++) {
        pos = 0;
        neg = 0;
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g1 > m) break;
            if (k & 1) {
                pos += p[m - g1];
                if (g2 <= m) pos += p[m - g2];
            } else {
                neg += p[m - g1];
                if (g2 <= m) neg += p[m - g2];
            }
        }
        p[m] = pos - neg;
    }

    pos = 0;
    neg = 0;
    for (d = 1; d <= n; d++) {
        if (n % d == 0) {
            int mu = mu_local((uint64_t)d);
            if (mu == 1) pos += p[n / d];
            else if (mu == -1) neg += p[n / d];
        }
    }
    result = pos - neg;
    free(p);
    return result;
}
