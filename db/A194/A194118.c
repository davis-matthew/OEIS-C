/*
 * A194118  Triangular array: T(n,k)=C(n+2,k)+C(n+2,k+2), 0<=k<=n.
 * https://oeis.org/A194118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(r+2,k) + C(r+2,k+2) where r = floor((sqrt(8n+1)-1)/2)
 *            (largest r with r*(r+1)/2 ≤ n), k = n - r*(r+1)/2
 * Range:     u8 n <= 38, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 59 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static idx_t find_row(idx_t n)
{
    /* binary search for the largest r with r*(r+1)/2 <= n */
    idx_t lo = 0, hi = n;
    idx_t best = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n) {
            best = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    unsigned __int128 start128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;
    idx_t k = n - start;

    seq_t term1 = binomial(r + 2, k);
    seq_t term2 = binomial(r + 2, k + 2);
    return term1 + term2;
}
