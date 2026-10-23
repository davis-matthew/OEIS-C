/*
 * A035041  a(n) = 2^n - C(n,0) - C(n,1) - ... - C(n,8).
 * https://oeis.org/A035041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - C(n,0) - C(n,1) - ... - C(n,8)
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static seq_t binom_small(idx_t n, idx_t k)
{
    /* Compute binomial(n, k) for k <= 8 using multiplicative formula */
    if (k > n) return 0;
    seq_t res = 1;
    idx_t i;
    for (i = 1; i <= k; ++i) {
        /* res = res * (n - k + i) / i */
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t total = pow2(n);

    /* Subtract binomials C(n, k) for k = 0..8 */
    if (n <= 8) {
        /* All binomials up to n are added; result will be zero */
    }

    idx_t k;
    for (k = 0; k <= 8; ++k) {
        if (n < k) break;
        total -= binom_small(n, k);
    }
    return total;
}
