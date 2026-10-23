/*
 * A014463  Triangular array formed from elements to left of middle of rows of
 *          Pascal's triangle that are not 1.
 * https://oeis.org/A014463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   a(n) is the n‑th element of the triangular array of binomial
 *            coefficients C(r,k) with r ≥ 3, 1 ≤ k ≤ ⌊(r‑1)/2⌋, enumerated by
 *            rows. The mapping from n to (r,k) is: r = smallest integer ≥ 3
 *            with Σ_{i=3}^r ⌊(i‑1)/2⌋ ≥ n k = n – Σ_{i=3}^{r‑1} ⌊(i‑1)/2⌋
 *            (1‑based within the row) The value returned is...
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n == 0) {          /* undefined for offset=1, but return 0 for safety */
        return 0;
    }
    idx_t row = 3;         /* first row contributing terms */
    idx_t cum = 0;         /* cumulative count of terms before current row */

    /* Find the row that contains the n‑th term */
    while (1) {
        idx_t terms_in_row = (row - 1) / 2;          /* floor((row‑1)/2) */
        if (cum + terms_in_row >= n) {
            /* The term is in this row */
            idx_t k = n - cum;                      /* 1‑based index in row */
            return binom(row, k);
        }
        cum += terms_in_row;
        ++row;
    }
}
