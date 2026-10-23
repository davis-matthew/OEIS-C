/*
 * A130301  Triangle read by rows: A130296 * A007318, as infinite lower
 *          triangular matrices.
 * https://oeis.org/A130301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = triangle entry A130301 flattened row‑major. Triangle
 *            entry (m,n) (1‑based indices) is: if n == 1: 2*m - 1 else:
 *            C(m-1,n-1) + C(m-1,n) where C is the binomial coefficient. The
 *            sequence a(n) is obtained by finding the unique row m such that
 *            m(m+1)/2 >= n, then column c = n - m(m-1)/2
 * Range:     u8 n <= 39, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Integer square root of a 128‑bit unsigned value.  Returns floor(sqrt(v)). */
static idx_t isqrt_u128(unsigned __int128 v)
{
    idx_t lo = 0;
    idx_t hi = ((idx_t)1) << 32;          /* > sqrt(2^67), safe upper bound */
    idx_t res = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v) {
            res = mid;
            lo  = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find row m such that m(m+1)/2 >= n. */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    idx_t r  = isqrt_u128(val);          /* floor(sqrt(8n+1)) */
    idx_t m  = (r - 1) >> 1;             /* first approximation */
    while ((unsigned __int128)m * (m + 1) / 2 < n) ++m;

    /* Column within the row (1‑based). */
    idx_t prev_sum = (unsigned __int128)m * (m - 1) / 2;
    idx_t col = n - prev_sum;

    if (col == 1)
        return (seq_t)(2 * m - 1);

    seq_t a1 = binomial(m - 1, col - 1);
    seq_t a2 = binomial(m - 1, col);
    return a1 + a2;
}
