/*
 * A184622  a(n) = floor(n*r+h), where r=sqrt(2), h=-1/3; complement of
 *          A184623.
 * https://oeis.org/A184622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(2) - 1/3)
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes floor(sqrt(2*n^2)) and adjusts based on the fractional part. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic. */
    seq_t nn = (seq_t)n;

    /* M = 2 * n^2 */
    seq_t M = 2 * nn * nn;

    /* k = floor(sqrt(M)) = floor(n * sqrt(2)) */
    seq_t k = floor_sqrt(M);

    /* Check if fractional part of sqrt(M) (i.e., sqrt(2)*n) is >= 1/3. */
    /* 9*M >= 9*k*k + 6*k + 1  <=>  sqrt(M) >= k + 1/3  */
    seq_t left  = 9 * M;
    seq_t right = 9 * k * k + 6 * k + 1;

    if (left >= right)
        return k;
    else
        return k - 1;
}
