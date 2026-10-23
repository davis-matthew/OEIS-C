/*
 * A131430  Row sums of triangle A131429.
 * https://oeis.org/A131430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n, n) - (n+1) + Sum_{k=0..n} binomial(2*k,
 *            k)/(k+1)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Compute the n-th term of OEIS sequence A131430. */
seq_t a(idx_t n)
{
    /* Catalan numbers C_0 = 1, C_{i} = C_{i-1} * (4*i - 2) / (i+1)  (i>=1)  */
    /* Cumulative sum S_n = Σ_{k=0..n} C_k  */
    seq_t cat = (seq_t)1;          /* C_0  */
    seq_t sum = cat;               /* S_0  */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t prev = cat;
        seq_t numerator   = (seq_t)(4 * i - 2);
        seq_t denominator = (seq_t)(i + 1);
        cat = prev * numerator / denominator;   /* exact division */
        sum += cat;                              /* update cumulative sum */
    }

    seq_t binom = cat * (seq_t)(n + 1);          /* binomial(2*n, n) = C_n * (n+1) */
    return binom - (seq_t)(n + 1) + sum;
}
