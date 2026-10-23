/*
 * A174474  a(n) = (2*n^2 - 2*n - 3)/8 + 3*(-1)^n*(1-2*n)/8.
 * https://oeis.org/A174474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n^2 - 2*n - 3)/8 + 3*(-1)^n*(1-2*n)/8
 * Range:     i8 n <= 8, i16 n <= 193, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index to the appropriate integer type */
    seq_t n_t = (seq_t)n;

    /* (-1)^n : 1 for even n, -1 for odd n */
    int sign = (n & 1) ? -1 : 1;

    /* Compute 2*n^2 - 2*n - 3 */
    seq_t first = 2 * n_t * n_t - 2 * n_t - 3;

    /* Compute 1 - 2*n */
    seq_t alt = 1 - 2 * n_t;

    /* Compute the alternating term: 3*(-1)^n*(1-2*n) */
    seq_t second = (seq_t)sign * 3 * alt;

    /* Sum the two parts and divide by 8 (exact division) */
    seq_t result = (first + second) / 8;

    return result;
}
