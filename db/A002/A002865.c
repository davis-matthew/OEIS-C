/*
 * A002865  Number of partitions of n that do not contain 1 as a part.
 * https://oeis.org/A002865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = p(n) - p(n-1) for n >= 1, a(0) = 1, where p(n) =
 *            A000041(n) is computed by Euler's pentagonal recurrence p(m) =
 *            Sum_{k>=1} (-1)^(k+1) * (p(m - k*(3k-1)/2) + p(m - k*(3k+1)/2)),
 *            p(0) = 1
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 151, u64 n <= 465,
 *            u128 n <= 1565, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;

    seq_t *p = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
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

    seq_t result = p[n] - p[n - 1];
    free(p);
    return result;
}
