/*
 * A055586  Sixth column of triangle A055584.
 * https://oeis.org/A055586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{j=0..n} (n-j+1) * binom(j+3,3) * 2^j Recurrence: let
 *            coeff(j) = binom(j+3,3) * 2^j and S(j) = sum_{k=0..j} coeff(k).
 *            Then a(0) = 1, for j ≥ 1: a(j) = a(j-1) + S(j-1) + coeff(j),
 *            where S(j) = S(j-1) + coeff(j). The algorithm computes these
 *            values iteratively in O(n)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 19, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, used by idx_t */
#include <stdlib.h>   /* for NULL */

seq_t a(idx_t n)
{
    /* Handle the trivial base case */
    if (n == 0)
        return (seq_t)1;

    /* Variables used in the recurrence */
    seq_t binom        = (seq_t)1;   /* binom(j+3,3) for current j */
    seq_t pow2         = (seq_t)1;   /* 2^j */
    seq_t coeff        = (seq_t)1;   /* current coefficient */
    seq_t sum_coeff    = (seq_t)1;   /* sum of coeff(k) for k ≤ j-1 */
    seq_t a_prev       = (seq_t)1;   /* a(j-1) */
    seq_t a_j          = (seq_t)0;   /* a(j) */

    /* Iterate from j = 1 up to n */
    for (idx_t j = 1; j <= n; ++j)
    {
        /* Update binomial coefficient:
           binom(j+3,3) = binom(prev) * (j+3) / j
           (the division is exact) */
        binom = (binom * (seq_t)(j + 3)) / (seq_t)j;

        /* Update power of two: 2^j */
        pow2 <<= 1;

        /* Compute coefficient for this j */
        coeff = binom * pow2;

        /* Recurrence for a(j) */
        a_j = a_prev + sum_coeff + coeff;

        /* Accumulate for next iteration */
        sum_coeff += coeff;
        a_prev     = a_j;
    }

    return a_prev;  /* a(n) now stored in a_prev */
}
