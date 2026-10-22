/*
 * A001979  Number of partitions of floor(7n/2) into n nonnegative integers
 *          each no more than 7.
 * https://oeis.org/A001979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [q^floor(7n/2)] of the Gaussian binomial C(n+7,7)_q =
 *            Product_{i=1..7} (1-q^(n+i))/(1-q^i)
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 182, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    size_t W = (size_t)((7 * (uint64_t)n) / 2);
    size_t k, i;
    seq_t *c = (seq_t *)malloc((W + 1) * sizeof(seq_t));
    seq_t result;

    for (k = 0; k <= W; k++) c[k] = 0;
    c[0] = 1;

    for (i = 1; i <= 7; i++) {
        size_t m;
        /* divide by (1 - q^i) */
        for (k = i; k <= W; k++) c[k] += c[k - i];
        /* multiply by (1 - q^(n+i)) */
        m = (size_t)n + i;
        if (m <= W) {
            for (k = W; k >= m; k--) {
                c[k] -= c[k - m];
                if (k == m) break;
            }
        }
    }

    result = c[W];
    free(c);
    return result;
}
