/*
 * A144303  Square array A(n,m), n>=0, m>=0, read by antidiagonals: A(n,m) =
 *          n-th number of the m-th iteration of the hyperbinomial transform
 *          on the sequence of 1's.
 * https://oeis.org/A144303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,m) = Σ_{j=0}^{n-1} m*(n-j+m)^(n-j‑1)*C(n,j) + 1, * where m
 *            is the column (iteration number) and n is the row. * The
 *            flattened sequence a(k) is obtained by reading the square array
 *            * A(n,m) by anti‑diagonals
 * Range:     u8 n <= 24, u16 n <= 41, u32 n <= 87, u64 n <= 186,
 *            u128 n <= 462, big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>              /* for size_t */
#include "../utils/seq_pow.h"    /* seq_pow(base,exp)  */
#include "../utils/binomial.h"  /* binomial(n,k)      */

/* Compute the entry A(row,col). */
static seq_t A_value(idx_t row, idx_t col)
{
    /* All entries on row 0 and column 0 are 1. */
    if (row == 0 || col == 0) return 1;

    seq_t total = 0;
    /* Sum j = 0 .. row-1.                     */
    /* term_j = col * (row-j+col)^(row-j-1) * C(row,j)   */
    for (idx_t j = 0; j < row; ++j) {
        seq_t base     = (seq_t)(row - j + col);
        seq_t pow_val  = seq_pow(base, row - j - 1);
        seq_t comb     = binomial(row, j);
        seq_t term     = (seq_t)col;
        term *= pow_val;
        term *= comb;
        total += term;
    }
    /* The j = row term contributes 1 (col * col^(−1) = 1). */
    total += 1;
    return total;
}

/* Return the flattened sequence a(n).  The index n corresponds to the
 * position in the array read by anti‑diagonals.
 */
seq_t a(idx_t n)
{
    /* Find s such that T(s) ≤ n < T(s+1),  where  T(s) = s*(s+1)/2. */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n) ++s;

    idx_t offset = n - s * (s + 1) / 2;
    idx_t row    = offset;          /* term index within the column */
    idx_t col    = s - offset;      /* iteration index           */
    return A_value(row, col);
}
