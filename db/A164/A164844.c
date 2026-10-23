/*
 * A164844  Generalized Pascal Triangle - satisfying the same recurrence as
 *          Pascal's triangle, but with a(n,0)=1 and a(n,n)=10^n (instead of
 *          both being 1).
 * https://oeis.org/A164844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n,k) = Sum_{i=0..k} 10^i * binomial(n-i-1,n-k-1), a(n,0)=1,
 *            a(n,n)=10^n. The function a(n) returns the row‑major flattening
 *            of this triangle: a(0)=1, a(1)=1, a(2)=10, a(3)=1, a(4)=11,
 *            a(5)=100,
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 64, u64 n <= 229, u128 n <= 818,
 *            big any n
 * Helpers:   binom, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"   /* seq_t binom(idx_t n, idx_t k); */
#include "../utils/pow10.h"   /* seq_t pow10(idx_t e); */

/* Determine (row,col) from the linear index n.  Uses binary search on
   the triangular numbers.  All arithmetic fits in idx_t for 64‑bit
   indices. */
static void get_row_col(idx_t n, idx_t *row, idx_t *col)
{
    idx_t low = 0;
    idx_t high = n;                 /* upper bound for row */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    *row = low;
    *col = n - low * (low + 1) / 2;
}

seq_t a(idx_t n)
{
    idx_t row, col;
    get_row_col(n, &row, &col);

    /* boundary values */
    if (col == 0)           /* a(row,0) = 1 */
        return (seq_t)1;
    if (col == row)         /* a(row,row) = 10^row */
        return pow10(row);

    seq_t sum = (seq_t)0;
    /* compute Sum_{i=0..col} 10^i * binom(row-i-1, row-col-1) */
    for (idx_t i = 0; i <= col; ++i) {
        seq_t term10 = pow10(i);                           /* 10^i   */
        idx_t n1 = row - i - 1;                            /* n-i-1  */
        idx_t k1 = row - col - 1;                          /* n-k-1  */
        seq_t c = binom(n1, k1);                           /* binomial */
        sum += term10 * c;
    }
    return sum;
}
