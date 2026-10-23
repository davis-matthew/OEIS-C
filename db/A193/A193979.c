/*
 * A193979  Triangular array: the self-fission of (p(n,x)), where
 *          p(n,x)=x*p(n-1,x)+n, with p(0,x)=1.
 * https://oeis.org/A193979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   offset_row_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (derived from the triangular array definition):
 *  Let n be the overall index.
 *  Find the unique row r and column c such that
 *      r(r+1)/2 ≤ n < (r+1)(r+2)/2   and   c = n - r(r+1)/2.
 *  Then
 *      a(n) = (1 + Tri(c)) * r + 1 - Tri2(c)
 *
 *  where
 *      Tri(c)  = c(c+1)/2          (0‑based triangular number)
 *      Tri2(c) = c(c-1)(c-2)/6
 *
 *  This follows from the self‑fission of the polynomial sequence
 *  p(n,x) = x·p(n‑1,x) + n with p(0,x) = 1.
 *  The resulting triangular array T[r][c] (r ≥ 0, 0 ≤ c ≤ r) satisfies
 *      T[r][c] = (1 + c(c+1)/2)·r + 1 – c(c-1)(c-2)/6
 *  and a(n) is the linear flattening of T[r][c] in row order (row 0, then row 1, …).   */

#include <stdint.h>
#include <math.h>
#include "../utils/offset_row_col.h"

seq_t a(idx_t n)
{
    idx_t r, c;
    offset_row_col(n, &r, &c);

    /* Triangular numbers using 128‑bit intermediates for safety */
    unsigned __int128 tri_c   = (unsigned __int128)c * (c + 1) / 2;            /* Tri(c)   */
    seq_t          slope     = (seq_t)(1 + tri_c);                          /* 1 + Tri(c) */
    seq_t          term1     = (seq_t)r * slope;                            /* r * (1+Tri(c)) */

    seq_t tmp = 0;
    if (c >= 3) {
        unsigned __int128 prod = (unsigned __int128)c * (c - 1) * (c - 2);
        tmp = (seq_t)(prod / 6);                                            /* Tri2(c) */
    }

    return term1 + 1 - tmp;
}
