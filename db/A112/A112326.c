/*
 * A112326  Triangle read by rows: T(n,k)=2^k*binomial(2n-k,n-k), 1<=k<=n.
 * https://oeis.org/A112326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^k * binomial(2r-k, r-k) where r is the unique integer
 *            with r(r-1)/2 < n ≤ r(r+1)/2, and k = n - r(r-1)/2 (1‑based
 *            indices)
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1: a(1) is the first term. */
    if (n == 0) return 0;     /* undefined, return 0 for robustness */

    /* Find the row number r by accumulating triangular numbers. */
    idx_t r = 1;
    idx_t cumulative = 1;     /* r(r+1)/2 after the loop sets r */
    while (cumulative < n) {
        r++;
        cumulative += r;
    }

    /* Number of elements before row r. */
    idx_t prev = cumulative - r;       /* (r-1)*r/2 */
    /* Column index k (1‑based) within the row. */
    idx_t k = n - prev;

    seq_t bin = binomial(2 * r - k, r - k);
    seq_t pow = pow2(k);
    return bin * pow;
}
