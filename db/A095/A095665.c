/*
 * A095665  Tenth column (m=9) of (1,3)-Pascal triangle A095660.
 * https://oeis.org/A095665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+8,8)*(n+27)/9 (equivalently, a(n) =
 *            3*binomial(n+9,9) - 2*binomial(n+8,9))
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute binomial(N,K) for very small K (here K≤9) using multiplicative
   method that always divides exactly.  N is an idx_t; result is seq_t. */
static seq_t binom_small(idx_t N, idx_t K)
{
    if (K > N) return (seq_t)0;
    if (K > N - K) K = N - K;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= K; ++i) {
        res = res * (seq_t)(N - K + i); /* multiply by numerator */
        res = res / (seq_t)i;           /* divide by denominator */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* binomial(n+8,8) and binomial(n+9,9) are needed */
    seq_t b_n   = binom_small((idx_t)n + 9u, 9u); /* binomial(n+9,9) */
    seq_t b_nm1 = binom_small((idx_t)n + 8u, 9u); /* binomial(n+8,9) */

    /* a(n) = 3*b_n - 2*b_nm1  (all operations fit in seq_t) */
    seq_t res = 3u * b_n - 2u * b_nm1;
    return res;
}
