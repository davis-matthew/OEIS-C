/*
 * A109984  a(n) = number of steps in all Delannoy paths of length n.
 * https://oeis.org/A109984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=n..2*n} k * binomial(n, 2*n - k) * binomial(k, n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n) = sum_{k=n..2*n} k * binomial(n, j) * binomial(n+j, n)
   where j = 2*n-k ranges from n down to 0.  The implementation computes the
   needed binomial coefficients iteratively, avoiding large intermediate
   factorials.  It works for any seq_t / idx_t combination. */

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0)
        return 0;

    /* bin_j = binomial(n, j) with j starting at n (so bin_j = 1) */
    seq_t bin_j = 1;

    /* b_k = binomial(k, n) with k starting at n (so b_k = 1) */
    seq_t b_k = 1;

    seq_t sum = 0;
    idx_t j = n;

    /* Iterate k from n to 2*n */
    for (idx_t k = n; k <= 2 * n; ++k)
    {
        /* term = k * bin_j * b_k */
        seq_t term = (seq_t)k * bin_j * b_k;
        sum += term;

        if (k < 2 * n) /* prepare for next iteration */
        {
            /* Update bin_j for j-1: binomial(n, j-1) = binomial(n, j) * j / (n-j+1) */
            bin_j = bin_j * (seq_t)j / (seq_t)(n - j + 1);
            --j;                                 /* decrement j for next step */

            /* Update b_k for k+1: binomial(k+1, n) = binomial(k, n) * (k+1) / (k+1-n) */
            b_k = b_k * (seq_t)(k + 1) / (seq_t)(k + 1 - n);
        }
    }

    return sum;
}
