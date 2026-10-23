/*
 * A194119  Triangular array: T(n,k)=C(n+3,k)+C(n+3,k+3), 0<=k<=n.
 * https://oeis.org/A194119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = C(r+3,k) + C(r+3,k+3), enumerated
 *            row-wise. For n ≥ 0, let r be the unique integer with r*(r+1)/2
 *            ≤ n < (r+1)*(r+2)/2, and let k = n – r*(r+1)/2. Then a(n) =
 *            C(r+3,k) + C(r+3,k+3)
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.  */
    idx_t r = 0;
    while (((seq_t)r * (r + 1)) / 2 <= (seq_t)n) {
        ++r;
    }
    --r;

    /* Position within the r‑th row. */
    seq_t before = ((seq_t)r * (r + 1)) / 2;
    idx_t k = n - (idx_t)before;

    /* Compute the two binomial coefficients. */
    seq_t v1 = binomial(r + 3, k);
    seq_t v2 = binomial(r + 3, k + 3);

    return v1 + v2;
}
