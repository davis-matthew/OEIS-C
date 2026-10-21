/*
 * A067176  A triangle of generalized Stirling numbers: sum of consecutive
 *          terms in the harmonic sequence multiplied by the product of their
 *          denominators.
 * https://oeis.org/A067176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 90, u64 n <= 230,
 *            u128 n <= 594, big any n
 * Helpers:   a_row_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A067176 – flattened triangle of generalized Stirling numbers.
 *  a(n) is the (r,k) entry such that n = r*(r+1)/2 + k,
 *  with 0 ≤ k ≤ r and a(r,k) defined by
 *
 *      a(n,k) = a(n-1,k) * n  +  (n-1)! / k!
 *      a(k,k) = 0
 *
 *  This implements the recurrence directly, avoiding the need to build
 *  the entire triangle or large factorials.
 */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/a_row_k.h"

/* Find the row r containing index n in the flattened triangle.
 * The cumulative number of entries up to row r-1 is r*(r+1)/2.
 */
static idx_t triangle_row(idx_t n)
{
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;
    return r;
}

/* Public entry point – returns the a(n) term of the flattened triangle. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Locate the row and column of the nth element. */
    idx_t r     = triangle_row(n);
    idx_t start = r * (r + 1) / 2;       /* first index of row r      */
    idx_t k     = n - start;              /* column index within row   */

    /* Compute the value using the recurrence. */
    return a_row_k(r, k);
}
