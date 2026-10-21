/*
 * A056951  Triangle whose rows show the result of flipping the first, first
 *          two, ... and finally first n coins when starting with the stack
 *          (1,2,3,4,...,n) [starting with all heads up, where signs show
 *          whether particular coins end up heads or tails].
 * https://oeis.org/A056951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) enumerates the entries of the triangle * whose rows are
 *            constructed by successive coin flips. * Each entry is given by *
 *            T(n,k) = 2k - n - b * with 1 ≤ k ≤ n and * b = 2 if 2k ≤ n + 1,
 *            otherwise b = 1. * The function a(n) returns the nth triangular
 *            entry * (row-major order, 1‑based indices)
 * Range:     i8 n <= 94, i16 n <= 94, i32 n <= 94, i64 n <= 94, i128 n <= 94,
 *            big any n
 * Helpers:   row_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/row_col.h"

seq_t a(idx_t n)
{
    /* Determine the row (r) and column (k) of the nth element */
    idx_t r, k;
    row_col(n, &r, &k);

    /* Compute the triangle entry using the formula */
    seq_t two_k = (seq_t)2 * (seq_t)k;
    seq_t b = ( 2 * k <= r + 1 ) ? (seq_t)2 : (seq_t)1;
    return two_k - (seq_t)r - b;
}
