/*
 * A127723  Floor of square root of sum of squares of the first n consecutive
 *          even numbers.
 * https://oeis.org/A127723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(4 * sum_{k=1..n} k^2)) = floor(sqrt( (n)*(n +
 *            1)*(2*n + 1) * 4 / 6 ))
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* the sequence is defined for n ≥ 1; a(0) is undefined but
       returning 0 for safety when called with n == 0. */
    if (n == 0)
        return (seq_t)0;

    seq_t N = (seq_t)n;
    seq_t prod = N * (N + (seq_t)1);          /* n*(n+1) */
    prod *= (2 * N + (seq_t)1);              /* * (2n+1) */
    prod *= (seq_t)4;                        /* * 4 */
    prod /= (seq_t)6;                        /* / 6 */
    return floor_sqrt(prod);
}
