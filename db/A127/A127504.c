/*
 * A127504  Triangle T(n,k) = phi(n) if k|n, =0 otherwise.
 * https://oeis.org/A127504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = the flattened triangle T(r,k) where * T(r,k) = phi(r) if
 *            k | r, otherwise 0. The index n runs * row‑by‑row with offset 1.
 *            The implementation finds the * row r in which n lies and returns
 *            the appropriate entry
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1) / 2) < n) {
        ++r;
    }

    /* Column index within the row (1‑based) */
    idx_t k = n - ((r - 1) * r / 2);

    /* Return phi(r) if k divides r, else 0 */
    if (r % k == 0) {
        return phi(r);
    } else {
        return (seq_t)0;
    }
}
