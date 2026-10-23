/*
 * A174963  Determinant of the symmetric n X n matrix M_n where M_n(j,k) = n
 *          for j = k, M_n(j,n) = n-j, M_n(n,k) = n-k, M_n(j,k) = 0 otherwise.
 * https://oeis.org/A174963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n^n - ((n-1)*n*(2*n-1)/6)*n^(n-2) (n >= 1)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Special case for n = 1, where n-2 would be negative */
    if (n == 1)
        return (seq_t)1;

    seq_t N = (seq_t)n;                    /* cast index to seq_t once */

    /* n^n */
    seq_t p1 = big_pow(N, N);

    /* coefficient ((n-1)*n*(2*n-1)/6) */
    seq_t coeff = ((seq_t)(n - 1)) * N * (((seq_t)2 * N) - 1) / 6;

    /* n^(n-2) */
    seq_t p2 = big_pow(N, (seq_t)(N - 2));

    return p1 - coeff * p2;
}
