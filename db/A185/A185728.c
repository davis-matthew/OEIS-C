/*
 * A185728  Array associated with squares, by antidiagonals.
 * https://oeis.org/A185728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the entry T(row,col) of the array defined by

       T(n,k) = (k-1)^2 + n   if n <  k,
                n^2           if n == k,
                n^2 - k       if n >  k,

   where (row,col) are obtained by traversing the infinite
   array anti‑diagonally (sum of indices constant) and listing
   each diagonal with decreasing column index.  The function
   computes this directly by solving for the diagonal, then
   applying the above piecewise formula.  No external tables
   or sieves are required.  The algorithm runs in O(1) time. */

#include <math.h>

seq_t a(idx_t n)
{
    /* Find the diagonal s (starting at 1) such that
       triangular(s-1) < n <= triangular(s).               */
    long double nd = (long double)n;
    uint64_t s = (uint64_t)((sqrtl(8.0L * nd + 1.0L) - 1.0L) / 2.0L);

    /* Correct possible off–by–one errors from floating point
       approximation.  Use 128‑bit arithmetic for safety.    */
    while (1) {
        unsigned __int128 tri_s  = (unsigned __int128)s * (s + 1) / 2;
        if (tri_s < n) {               /* too small */
            ++s;
            continue;
        }
        unsigned __int128 tri_prev = (unsigned __int128)(s - 1) * s / 2;
        if (tri_prev >= n) {           /* too big, step back */
            --s;
            continue;
        }
        break;                          /* correct s */
    }

    /* Position within diagonal: 1-based */
    unsigned __int128 tri_prev = (unsigned __int128)(s - 1) * s / 2;
    idx_t r = n - (idx_t)tri_prev;    /* 1 <= r <= s */

    /* Convert to array indices (1-based) */
    idx_t row_idx = r;
    idx_t col_idx = s + 1 - r;

    /* Work in seq_t arithmetic to avoid overflow */
    seq_t row = (seq_t)row_idx;
    seq_t col = (seq_t)col_idx;

    if (row < col) {
        seq_t km1 = col - 1;           /* (k-1) in seq_t */
        return km1 * km1 + row;        /* (k-1)^2 + n */
    } else if (row == col) {
        return row * row;              /* n^2   */
    } else {                           /* row > col */
        return row * row - col;        /* n^2 - k */
    }
}
