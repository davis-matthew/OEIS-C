/*
 * A050157  T(n, k) = S(2n, n, k) for 0<=k<=n and n>=0, where S(p, q, r) is
 *          the number of upright paths from (0, 0) to (p, p-q) that do not
 *          rise above the line y = x-r.
 * https://oeis.org/A050157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2r choose r) - (2r choose r+k+1) where r is the largest
 *            integer such that r(r+1)/2 ≤ n, and k = n – r(r+1)/2. This
 *            yields the entry T(r,k) of the OEIS triangle, returned in
 *            row‑major order
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

/* Find the row index r for a given flat index n.
   r satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
static inline idx_t row_for_n(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* Exponentially grow hi until the inequality fails. */
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n)
        hi <<= 1;
    /* Binary search between lo and hi. */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1ULL) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Determine row and column in the triangle. */
    idx_t r = row_for_n(n);
    idx_t k = n - (unsigned __int128)r * (r + 1) / 2;

    /* Compute the binomial terms. */
    idx_t two_r = 2 * r;
    seq_t bin1 = binomial(two_r, r);
    seq_t bin2 = binomial(two_r, r + k + 1);

    return bin1 - bin2;
}
