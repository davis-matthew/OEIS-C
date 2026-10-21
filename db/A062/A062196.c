/*
 * A062196  Triangle read by rows, T(n, k) = binomial(n, k)*binomial(n + 2,
 *          k).
 * https://oeis.org/A062196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(m,k) where m = floor((sqrt(8*n+1)-1)/2), k = n -
 *            m*(m+1)/2, T(m,k) = binomial(m,k) * binomial(m+2,k) (OEIS
 *            A062196)
 * Range:     u8 n <= 17, u16 n <= 58, u32 n <= 178, u64 n <= 611,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static inline idx_t triangle_row(idx_t n)
{
    /* Find largest m such that m*(m+1)/2 <= n (count of elements
       before row m).  Uses 128‑bit arithmetic to avoid overflow. */
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 v = (unsigned __int128)mid * (mid + 1) / 2;
        if (v <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t m = triangle_row(n);
    /* Compute the offset within the row.           */
    idx_t k = n - (idx_t)((unsigned __int128)m * (m + 1) / 2);

    seq_t term1 = binomial(m, k);
    seq_t term2 = binomial(m + 2, k);
    return term1 * term2;
}
