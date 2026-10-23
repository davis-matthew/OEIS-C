/*
 * A118978  Array read by antidiagonals: the n-th row contains the binomial
 *          transform of row n-1 of A014410.
 * https://oeis.org/A118978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{k=0}^{min(col,row-1)} C(col,k) * C(row+1,k+1) where
 *            (row,col) are obtained by reading the array a(n) antidiagonally.
 *            The array is built as: each row r (1‑based) is the binomial
 *            transform of row r-1 of A014410, which consists of the binomial
 *            coefficients C(r+1,k+1) for k=0..r-1
 * Range:     u8 n <= 48, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiplicative binomial coefficient C(n,k) for idx_t arguments. */
static inline seq_t binom_n_k(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    /* Find the diagonal (0‑based) containing the nth element. */
    unsigned __int128 reach = 0;   /* sum of lengths of previous diagonals. */
    idx_t diag = 0;
    while (reach + (unsigned __int128)(diag + 1) < (unsigned __int128)n) {
        reach += diag + 1;
        ++diag;
    }

    /* Offset within the diagonal (0‑based). */
    idx_t offset = n - (idx_t)reach - 1;

    /* Convert to (row, col) coordinates.
       Rows are numbered starting at 1, columns at 0. */
    idx_t row = diag + 1 - offset;   /* 1‑based row index. */
    idx_t col = offset;              /* 0‑based column index. */

    /* Sum over the non‑zero terms of the binomial transform. */
    idx_t limit = (col < row - 1U) ? col : row - 1U;   /* min(col, row-1). */
    seq_t result = 0;
    for (idx_t k = 0; k <= limit; ++k) {
        seq_t C_col_k  = binom_n_k(col,     k);
        seq_t C_row_kp = binom_n_k(row + 1, k + 1);
        result += C_col_k * C_row_kp;
    }
    return result;
}
