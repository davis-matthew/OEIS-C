/*
 * A236345  a(n) is the Manhattan distance between n and n^2 in a left-aligned
 *          triangle with next M natural numbers in row M: 1, 2 3, 4 5 6, 7 8
 *          9 10, etc.
 * https://oeis.org/A236345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |row(n) - row(n^2)| + |col(n) - col(n^2)|
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where row(x) and col(x) are the 1‑based row and column in a left‑aligned
 * triangle of size M in row M. */

/* Utility for integer square root of a seq_t value. */
#include "../utils/floor_sqrt.h"

/* Compute the 1‑based row and column of a number x in the triangle. */
static void row_col(seq_t x, idx_t *row, idx_t *col)
{
    /* Find smallest r with r(r+1)/2 >= x. */
    seq_t m = (seq_t)1 + (seq_t)8 * x;          /* 1 + 8*x */
    seq_t s = floor_sqrt(m);                   /* floor sqrt(1+8x) */
    idx_t r = (idx_t)((s - 1) / 2);             /* floor division */
    seq_t t = ((seq_t)r * (r + 1)) / 2;         /* T_r = r(r+1)/2 */
    if (t < x) r++;                            /* need ceil */
    *row = r;

    seq_t prev = ((seq_t)r * (r - 1)) / 2;      /* T_{r-1} */
    *col = (idx_t)(x - prev);                  /* 1‑based column */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t n_sq = (seq_t)n * (seq_t)n;          /* n^2 */

    idx_t r1, c1, r2, c2;
    row_col((seq_t)n,  &r1, &c1);              /* row & col of n   */
    row_col(n_sq,     &r2, &c2);              /* row & col of n^2 */

    seq_t dr = (seq_t)(r1 > r2 ? r1 - r2 : r2 - r1);
    seq_t dc = (seq_t)(c1 > c2 ? c1 - c2 : c2 - c1);

    return dr + dc;
}
