/*
 * A022178  Triangle of Gaussian binomial coefficients [ n,k ] for q = 14.
 * https://oeis.org/A022178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Gaussian binomial coefficient [r,k] (q = 14) where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r*(r+1)/2. The triangle is
 *            read row‑wise, left to right, top to bottom
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 22, u64 n <= 47, u128 n <= 81,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains the n‑th element of the triangle. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index k within that row. */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;          /* 0 ≤ k ≤ r */

    /* Gaussian binomial coefficient [r,k] with q = 14. */
    if (k == 0 || k == r)
        return (seq_t)1;

    /* Use symmetry [r,k] == [r,r‑k] to reduce the number of terms. */
    idx_t kk = (k <= r - k) ? k : r - k;

    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= kk; ++i) {
        seq_t num = pow_int((seq_t)14, r + 1 - i) - (seq_t)1;
        seq_t den = pow_int((seq_t)14, i) - (seq_t)1;
        res = res * num / den;
    }
    return res;
}
