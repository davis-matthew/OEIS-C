/*
 * A028331  Elements to the right of the central elements of the even-Pascal
 *          triangle A028326 that are not 2.
 * https://oeis.org/A028331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 255, u64 n <= 1055,
 *            u128 n <= 2600, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Elements to the right of the central elements of the even‑Pascal triangle (A028331)
   a(n) = 2 * binomial(r + 3, k + 2 + floor((r + 1) / 2))
   where r is the row index such that
          Σ_{i=0}^{r-1} (floor(i / 2) + 1) ≤ n < Σ_{i=0}^{r} (floor(i / 2) + 1)
   and k = n - Σ_{i=0}^{r-1} (floor(i / 2) + 1) (0 ≤ k ≤ floor(r / 2)) */

/* The binomial helper is provided by the build system. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Handle the trivial case quickly */
        return (seq_t)6;
    }

    idx_t idx = n;
    idx_t r = 0;           /* current row index (will be incremented after the loop) */
    idx_t cum = 0;         /* cumulative number of terms up to row r-1 */

    /* Find the row r that contains the element at position idx. */
    while (cum <= idx) {
        idx_t terms_in_row = (r / 2) + 1;   /* floor(r/2) + 1  */
        cum += terms_in_row;
        r++;
    }
    r--;                      /* back to the correct row index */

    /* Number of terms in this row */
    idx_t terms_in_row = (r / 2) + 1;

    /* cum is now the total number of terms up to row r.
     * cum_prev is the total number of terms up to row r-1. */
    idx_t cum_prev = cum - terms_in_row;

    /* Index of the element within the row (0‑based) */
    idx_t k = idx - cum_prev;

    /* Compute the binomial argument j = k + 2 + floor((r+1)/2) */
    idx_t shift = (r + 1) / 2;       /* floor((r + 1)/2) */
    idx_t j = k + 2 + shift;

    seq_t val = binomial(r + 3, j);  /* binomial(r+3, j) */
    val = val * (seq_t)2;            /* multiply by 2 */

    return val;
}
