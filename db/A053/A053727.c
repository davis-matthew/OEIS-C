/*
 * A053727  Triangle T(n,k) = Sum_{d|gcd(n,k)} mu(d)*C(n/d,k/d) (n >= 1, 1 <=
 *          k <= n).
 * https://oeis.org/A053727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the linearisation of the triangle * T(r,k) =
 *            Σ_{d|gcd(r,k)} μ(d) · C(r/d , k/d) * with r = smallest integer
 *            such that r(r+1)/2 ≥ n * and k = n – r(r-1)/2 (1‑based column)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   binomial, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"
#include "../utils/mobius.h"

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but safe guard */

    /* Find the row r such that (r-1)r/2 < n ≤ r(r+1)/2  */
    double disc = sqrt((double)(8 * n + 1));
    idx_t r = (idx_t)((disc - 1.0) / 2.0);
    /* Correct possible off‑by‑one caused by floating point round‑off */
    if ((__uint128_t)r * (r + 1) / 2 < n) ++r;

    /* Compute column k (1‑based) */
    __uint128_t prev_sum128 = (__uint128_t)(r - 1) * r / 2;
    idx_t prev_sum = (idx_t)prev_sum128;
    idx_t k = n - prev_sum;

    /* Evaluate T(r,k) */
    idx_t g = gcd_idx(r, k);
    seq_t pos = 0, neg = 0;      /* separate positive/negative contributions */

    for (idx_t d = 1; d * d <= g; ++d) {
        if (g % d == 0) {
            int mu_d = (int)mobius(d);
            if (mu_d) {
                seq_t bin = binomial(r / d, k / d);
                if (mu_d > 0) pos += bin;
                else          neg += bin;
            }

            idx_t d2 = g / d;
            if (d2 != d) {
                int mu_d2 = (int)mobius(d2);
                if (mu_d2) {
                    seq_t bin2 = binomial(r / d2, k / d2);
                    if (mu_d2 > 0) pos += bin2;
                    else          neg += bin2;
                }
            }
        }
    }

    /* In theory pos ≥ neg; the subtraction is safe on unsigned types. */
    return pos - neg;
}
