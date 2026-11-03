/*
 * A083044  Square table read by antidiagonals forms a permutation of the
 *          natural numbers: T(n,0) = floor(n*x/(x-1))+1, T(n,k+1) =
 *          ceiling(x*T(n,k)), where x=3/2, n >= 0, k >= 0.
 * https://oeis.org/A083044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 67 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Implementation of OEIS sequence A083044.
 * The sequence is obtained by reading the square table T(n,k) by
 * antidiagonals.  The table is defined by
 *   T(n,0) = 3*n + 1
 *   T(n,k+1) = ceil( (3*T(n,k)) / 2 ) = (3*T(n,k)+1)/2
 * for n,k ≥ 0.  The function a(n) returns the n-th element of the
 * anti‑diagonal traversal of this table, with a(0)=1.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute T(row, col) by iterating the recurrence */
static inline seq_t table_value(idx_t row, idx_t col)
{
    seq_t val = (3 * (seq_t)row) + 1;          /* T(row,0)  */
    for (idx_t m = 0; m < col; ++m) {
        /* Use 128‑bit arithmetic to avoid overflow of the multiplication */
        val = (seq_t)(((__uint128_t)3 * val + 1) / 2);
    }
    return val;
}

/* a(n) = value at (i,j) where (i,j) are the coordinates of the
 * element n in the anti‑diagonal reading of T. */
seq_t a(idx_t n)
{
    /* Find the diagonal d such that (d+1)(d+2)/2 > n.  Use 128‑bit
     * arithmetic for the comparison to avoid overflow when n is large. */
    idx_t d = 0;
    while ((((__uint128_t)(d + 1) * (d + 2)) / 2) <= n) {
        ++d;
    }

    /* Number of elements before diagonal d */
    idx_t prev = (d * (d + 1)) / 2;
    /* Position inside the diagonal */
    idx_t i = n - prev;
    idx_t j = d - i;         /* since T(i, j) with i+j = d */

    return table_value(i, j);
}
