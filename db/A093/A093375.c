/*
 * A093375  Array T(m,n) read by ascending antidiagonals: T(m,n) =
 *          m*binomial(n+m-2, n-1) for m, n >= 1.
 * https://oeis.org/A093375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 142, u32 n <= 510, u64 n <= 2044,
 *            u128 n <= 4950, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A093375: read the triangle T(m,n)=m*binomial(n+m-2,n-1) by
   ascending antidiagonals.  a(n) is the (n‑1)‑st element of that
   ordering (n≥1).  This implementation determines the diagonal d
   containing position n by a simple integer loop, computes the
   coordinates (m, col) of the element, evaluates the binomial
   coefficient using the supplied binomial() helper, and returns
   m*binomial(col+m-2, col-1).  Complexity: O(√n) for the diagonal
   search, O(1) otherwise.  Works for any seq_t width because all
   arithmetic is performed in the supplied integer types. */
#include <stdint.h>
#include "../utils/binomial.h" /* binomial(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out‑of‑range → 0 */

    /* find the diagonal d such that d*(d+1)/2 ≥ n  */
    idx_t d = 1;
    /* use 128‑bit to avoid overflow on very large n  */
    while ((unsigned __int128)d * (d + 1) / 2 < n) ++d;

    /* number of elements strictly before this diagonal */
    idx_t prev = (unsigned __int128)(d - 1) * d / 2;

    /* position within the diagonal (0‑based) */
    idx_t offset = n - prev - 1;

    /* coordinates in the triangle */
    idx_t m   = d - offset;      /* row index ≥ 1 */
    idx_t col = offset + 1;      /* column index ≥ 1 */

    /* binomial(col + m - 2, col - 1) */
    seq_t bin = binomial(col + m - 2, col - 1);

    return (seq_t)m * bin;
}
