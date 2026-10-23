/*
 * A193451  Triangle of a binomial convolution sum related to Jacobsthal
 *          numbers.
 * https://oeis.org/A193451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A193451
   a(n) is the flattening of the triangle T(r,k) given by
   T(r,k) = sum_{j=0..k} (-1)^{j+k} (j+k) * binomial(r-k+j , j)
   where the triangle is output row‑wise starting from r = 0.
   Formula for the triangle entry:
       T(r,k) = Σ_{j=0..k} (-1)^{j+k} (j+k) * binomial(r-k+j , j)        */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find the row r and column k that correspond to the
       n‑th element in the row‑major listing of the triangle.   */
    idx_t r = 0;
    while ((((unsigned __int128)(r + 1)) * (r + 2)) / 2 <= n) {
        ++r;
    }

    /* previous number of elements before this row */
    idx_t prev = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - prev;          /* column index */

    /* Evaluate the alternating sum for T(r,k) using only
       unsigned arithmetic; keep positive and negative parts
       separate to avoid underflow.                     */
    seq_t pos = 0;               /* sum of positive terms       */
    seq_t neg = 0;               /* sum of negative terms       */

    for (idx_t j = 0; j <= k; ++j) {
        seq_t term = (seq_t)(j + k) * binomial(r - k + j, j);
        if (((j + k) & 1) == 0) {   /* even exponent → positive    */
            pos += term;
        } else {                   /* odd exponent  → negative    */
            neg += term;
        }
    }

    return pos - neg;            /* final value is non‑negative   */
}
