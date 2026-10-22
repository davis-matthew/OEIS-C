/*
 * A007042  Left diagonal of partition triangle A047812.
 * https://oeis.org/A007042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A000041(n+1) - 2, where A000041 is computed by Euler's
 *            pentagonal recurrence p(m) = Sum_{k>=1} (-1)^(k+1)
 *            (p(m-k(3k-1)/2) + p(m-k(3k+1)/2)), p(0) = 1
 * Range:     u8 n <= 15, u16 n <= 42, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t partition_count(idx_t n) {
    seq_t *p = alloc_zero(n + 1);
    p[0] = 1;
    for (idx_t m = 1; m <= n; m++) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            idx_t g2 = k * (3 * k + 1) / 2;
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
    seq_t result = p[n];
    release(p, n + 1);
    return result;
}

seq_t a(idx_t n) {
    return partition_count(n + 1) - 2;
}
