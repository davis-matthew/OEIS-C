/*
 * A001980  Number of partitions of floor(7n/2)-1 into n nonnegative integers
 *          each no greater than 7.
 * https://oeis.org/A001980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [q^w] prod_{i=1..7} (1-q^(n+i))/(1-q^i), w =
 *            floor(7n/2)-1 (Gaussian binomial [n+7 choose 7]_q; a(0) = 0)
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 182, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    idx_t w = (7 * n) / 2 - 1;
    seq_t *c = (seq_t *)malloc((size_t)(w + 1) * sizeof(seq_t));
    idx_t d;
    int i;
    for (d = 0; d <= w; d++) c[d] = 0;
    c[0] = 1;
    for (i = 1; i <= 7; i++) {
        /* divide by (1 - q^i): truncated series, all coefficients >= 0 */
        for (d = (idx_t)i; d <= w; d++) c[d] += c[d - (idx_t)i];
        /* multiply by (1 - q^(n+i)); result is a Gaussian polynomial, >= 0 */
        idx_t s = n + (idx_t)i;
        if (s <= w) {
            for (d = w; ; d--) {
                c[d] -= c[d - s];
                if (d == s) break;
            }
        }
    }
    seq_t r = c[w];
    free(c);
    return r;
}
