/*
 * A060621  Number of flips between the d-dimensional tilings of the unary
 *          zonotope Z(D,d). Here the codimension D-d is equal to 3 and d
 *          varies.
 * https://oeis.org/A060621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n*(n^2+11n+24)/2
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    /* Compute (n^2 + 11n + 24) / 2 */
    seq_t expr = n_t * n_t + (seq_t)11 * n_t + (seq_t)24;
    expr /= (seq_t)2;
    /* Compute 2^n */
    seq_t pow2 = (seq_t)1 << n;
    return pow2 * expr;
}
