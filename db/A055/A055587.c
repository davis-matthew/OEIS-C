/*
 * A055587  Triangle with columns built from row sums of the partial row sums
 *          triangles obtained from Pascal's triangle A007318. Essentially
 *          A049600 formatted differently.
 * https://oeis.org/A055587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute A055587, the flattened triangle defined by
 *  T(n,k) = Σ_{j=0}^{n-k} C(n-k,j)·C(k+j-1,k-1),  T(n,0)=1.
 *  The sequence lists the entries row‑by‑row:
 *  a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Return T(row,col).  row, col are 0‑based.  */
static seq_t
t_row_col(idx_t row, idx_t col)
{
    if (col == 0)          /* first element of every row is 1 */
        return (seq_t)1;

    /* T(row,col) = Σ_{j=0}^{row-col} C(row-col,j) * C(col+j-1,col-1) */
    idx_t a = row - col;          /* n-k */
    idx_t k = col;                /* k */
    seq_t sum = (seq_t)0;

    for (idx_t j = 0; j <= a; ++j) {
        seq_t term1 = binomial(a, j);
        seq_t term2 = binomial(k + j - 1, k - 1);
        sum += term1 * term2;
    }
    return sum;
}

/* Flattened index → (row,col) → value.  */
seq_t
a(idx_t n)
{
    /* Find the row r containing position n.  */
    idx_t r = 0;
    idx_t cum = 0;          /* total elements before current row r */

    /* Increment r until cum + (r+1) > n.  */
    while (cum + r + 1 <= n) {
        cum += r + 1;
        ++r;
    }

    /* Column within the row.  */
    idx_t col = n - cum;

    return t_row_col(r, col);
}
