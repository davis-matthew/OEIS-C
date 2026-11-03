/*
 * A101391  Triangle read by rows: T(n,k) is the number of compositions of n
 *          into k parts x_1, x_2, ..., x_k such that gcd(x_1,x_2,...,x_k) = 1
 *          (1<=k<=n).
 * https://oeis.org/A101391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = sum_{d|r} binomial(d-1,k-1) * mobius(r/d) where r =
 *            row_of(n-1)+1 and k = n - r*(r-1)/2 is the position within the
 *            r‑th row of the triangle defined by T(n,k) = Σ_{d|n}
 *            binomial(d-1,k-1) * mobius(n/d)
 * Range:     u8 n <= 70, u16 n <= 198, u32 n <= 647, u64 n <= 2377,
 *            u128 n <= 8841, big any n
 * Validated: standard, pass, 107 terms through n = 9693 (2026-11-01)
 * Helpers:   mobius_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius_idx.h"

static inline idx_t
row_of(idx_t n)                 /* return r with r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
{
    idx_t low  = 0;
    idx_t high = 1;
    /* exponential search to find an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        low  = high;
        high <<= 1;              /* double high */
    }
    /* binary search between low (inclusive) and high (inclusive) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1) + 1; /* upper mid to avoid infinite loop */
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static inline seq_t binom_small(idx_t n, idx_t k)
{
    /* Compute binomial(n,k) for small n (≤ 140) safely using 128‑bit
       arithmetic.  The result is guaranteed to fit in seq_t for all
       values that occur in our range of n. */
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;   /* use symmetry */

    __uint128_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (n - k + i);
        res /= i;               /* exact division */
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Determine row r and position k within that row. */
    idx_t r   = row_of(n - 1) + 1;          /* 1‑based row number  */
    idx_t prv = (r - 1) * r / 2;            /* number of elements before row r */
    idx_t k   = n - prv;                    /* 1‑based column number  */

    seq_t pos = 0;   /* Σ μ > 0 contributions     */
    seq_t neg = 0;   /* Σ μ < 0 contributions     */

    /* Enumerate all divisors d of r. */
    for (idx_t d = 1; d * d <= r; ++d) {
        if (r % d != 0) continue;

        /* First divisor d. */
        if (k - 1 <= d - 1) {
            int mu = mobius_idx(r / d);
            if (mu != 0) {
                seq_t b = binom_small(d - 1, k - 1);
                if (mu > 0) pos += b; else neg += b;
            }
        }

        /* Second divisor d2 = r/d, if distinct. */
        idx_t d2 = r / d;
        if (d2 != d) {
            if (k - 1 <= d2 - 1) {
                int mu = mobius_idx(r / d2);
                if (mu != 0) {
                    seq_t b = binom_small(d2 - 1, k - 1);
                    if (mu > 0) pos += b; else neg += b;
                }
            }
        }
    }

    /* The value is always non‑negative. */
    return pos - neg;
}
