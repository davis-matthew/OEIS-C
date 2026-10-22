/*
 * A001261  Number of permutations of length n with 5 consecutive ascending
 *          pairs.
 * https://oeis.org/A001261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n-1,5) * A000255(n-6), where A000255(k) =
 *            k*A000255(k-1) + (k-1)*A000255(k-2), A000255(0) = 1, A000255(1)
 *            = 1 (from E.g.f. (x^5/5!)*exp(-x)/(1-x)^2 with offset 1)
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 15, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 6) return 0;
    idx_t m = n - 1;      /* binomial(m,5) */
    idx_t k = n - 6;      /* index into A000255 */

    /* binomial(m,5) multiplicatively: c = C(m-5+i, i) after step i */
    seq_t c = 1;
    idx_t i;
    for (i = 1; i <= 5; i++) {
        c = c * (seq_t)(m - 5 + i);
        c = c / (seq_t)i;
    }

    /* A000255(k) */
    seq_t d0 = 1, d1 = 1;
    if (k == 0) return c * d0;
    idx_t j;
    for (j = 2; j <= k; j++) {
        seq_t d2 = (seq_t)j * d1 + (seq_t)(j - 1) * d0;
        d0 = d1;
        d1 = d2;
    }
    return c * d1;
}
