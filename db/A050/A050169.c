/*
 * A050169  Triangle read by rows: T(n,k) = gcd(C(n,k), C(n,k-1)), n >= 1, 1
 *          <= k <= n.
 * https://oeis.org/A050169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = gcd(C(r,k), C(r,k-1)) where r is the row with *
 *            triangular(r-1) < n <= triangular(r) and k = n - triangular(r-1)
 * Range:     u8 n <= 31, u16 n <= 179, u32 n <= 611, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

/* Triangular number: T(r) = r*(r+1)/2, computed in seq_t to avoid overflow. */
static inline seq_t triangular(idx_t r)
{
    return ((seq_t)r * (seq_t)(r + 1)) / 2;
}

seq_t a(idx_t n)
{
    /* Find the row r: triangular(r-1) < n <= triangular(r). */
    idx_t low  = 1;
    idx_t high = 1;

    /* Exponential search for an upper bound. */
    while (triangular(high) < (seq_t)n)
    {
        /* Avoid overflow of high; clamp to UINT64_MAX. */
        if (high > UINT64_MAX / 2) { high = UINT64_MAX; break; }
        high <<= 1;
    }

    /* Binary search for the smallest r with triangular(r) >= n. */
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (triangular(mid) < (seq_t)n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;

    /* Compute column index k. */
    seq_t prev_tri = triangular(r - 1);
    idx_t k = (idx_t)((seq_t)n - prev_tri);  /* 1 <= k <= r */

    /* Compute the two binomial coefficients. */
    seq_t c1 = binomial(r, k);         /* C(r, k)   */
    seq_t c2 = binomial(r, k - 1);     /* C(r, k-1) */

    /* Return their greatest common divisor. */
    return gcd(c1, c2);
}
