/*
 * A050156  T(n,k)=M(2n+3,n+3,k+3), 0<=k<=n, n >= 0, array M as in A050144.
 * https://oeis.org/A050156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) flattened element of the triangle T(r,k) =
 *            binom(2r+3, r-k) – binom(2r+3, r-k-1) where r =
 *            floor((sqrt(8n+1)-1)/2) and k = n - r(r+1)/2 (r is the row
 *            number, k the column in that row, 0‑based)
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t N) {
    /* Find the row r such that N lies in the r‑th row.
       The cumulative number of elements up to row r is
       (r+1)(r+2)/2.  We increase r until that value exceeds N. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= N)
        ++r;

    /* Position k inside the row. */
    idx_t k = N - r * (r + 1) / 2;

    /* Compute the triangular entry as the difference of successive
       binomial coefficients.  j = r - k is always non‑negative. */
    idx_t n1 = 2 * r + 3;          /* 2r+3 in the binomial argument */
    idx_t j  = r - k;              /* n-k in the binomial argument */

    seq_t top   = binomial(n1, j);
    seq_t bottom = (j > 0) ? binomial(n1, j - 1) : 0;

    return top - bottom;
}
