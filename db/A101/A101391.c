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
 * Helpers:   mobius, row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/row_of.h"
#include "../utils/mobius.h"

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
            int mu = mobius(r / d);
            if (mu != 0) {
                seq_t b = binom_small(d - 1, k - 1);
                if (mu > 0) pos += b; else neg += b;
            }
        }

        /* Second divisor d2 = r/d, if distinct. */
        idx_t d2 = r / d;
        if (d2 != d) {
            if (k - 1 <= d2 - 1) {
                int mu = mobius(r / d2);
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
