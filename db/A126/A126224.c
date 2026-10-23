/*
 * A126224  Determinant of the n X n matrix in which the entries are 1 through
 *          n^2, spiraling inward starting with 1 in the (1,1)-entry.
 * https://oeis.org/A126224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^{n*(n-1)/2} * (3*n-1) * (2*n-2)! / (2 * (n-1)!)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 14, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for bool */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Determine the sign: (-1)^{n*(n-1)/2} */
    bool negative = ((n * (n - 1) / 2) & 1ULL) != 0;

    /* Compute product_{k=n}^{2*n-2} k */
    seq_t prod = (seq_t)1;
    for (idx_t k = n; k <= 2 * n - 2; ++k)
        prod *= (seq_t)k;

    /* Multiply by (3*n-1) and divide by 2 */
    prod *= (seq_t)(3 * n - 1);
    prod /= (seq_t)2;

    return negative ? -(prod) : prod;
}
