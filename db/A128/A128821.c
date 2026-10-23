/*
 * A128821  Triangle T(n,k), 1<=k<=n, read by rows defined by
 *          :T(n,k)=C(n,k)*C(n-1,k-1)+C(n,k-1)*C(n-1,k)where
 *          C(n,k)=A007318(n,k) .
 * https://oeis.org/A128821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, column) where T(n,k) = C(n,k)*C(n-1,k-1) +
 *            C(n,k-1)*C(n-1,k) (1 ≤ k ≤ n, offset 1) The sequence is read by
 *            rows of the triangle T
 * Range:     u8 n <= 17, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≥ n and (r-1)*r/2 < n */
    idx_t low = 1, high = n, r = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t cum = (mid * (mid + 1)) >> 1;   /* cumulative elements up to row mid */
        if (cum >= n) {
            r = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    /* Column k: position within the row (1‑based) */
    idx_t prev_cum = ((r - 1) * r) >> 1;      /* cumulative elements before row r */
    idx_t k = n - prev_cum;                  /* 1≤k≤r */

    /* Compute the two binomial products */
    seq_t c1 = binomial(r, k);
    seq_t c2 = binomial(r - 1, k - 1);
    seq_t term1 = c1 * c2;                    /* C(r,k)*C(r-1,k-1) */

    seq_t c3 = binomial(r, k - 1);
    seq_t c4 = binomial(r - 1, k);
    seq_t term2 = c3 * c4;                    /* C(r,k-1)*C(r-1,k) */

    return term1 + term2;
}
