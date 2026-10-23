/*
 * A128884  Sum of all matrix elements of n X n Vandermonde matrix of numbers
 *          1,2,...,n, i.e., the matrix A with A[i,j] = i^(j-1), 1 <= i <= n,
 *          1 <= j <= n.
 * https://oeis.org/A128884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + Sum_{i=2..n} (i^n - 1)/(i-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* For n==1 the loop below is skipped, result stays as 1 */
    seq_t result = (seq_t)n;  /* contribution of i = 1 : sum_{j=1..n} 1^(j-1) = n */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t power   = pow_uint((uint64_t)i, n);        /* i^n          */
        seq_t numerator = power - 1;                     /* i^n - 1      */
        seq_t denom      = (seq_t)(i - 1);                /* i - 1        */
        seq_t term        = numerator / denom;           /* (i^n - 1)/(i-1) */
        result += term;
    }

    return result;
}
