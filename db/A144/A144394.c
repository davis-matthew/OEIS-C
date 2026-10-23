/*
 * A144394  Triangle read by rows (n >= 4, 0 <= k <= n - 4): row n gives the
 *          coefficients in the expansion of ((x + 1)^n - (x^n + n*x^(n - 1) +
 *          n*x + 1))/x^2.
 * https://oeis.org/A144394
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 129, u32 n <= 514, u64 n <= 2112,
 *            u128 n <= 6906, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A144394 – “Triangle read by rows (n ≥ 4, 0 ≤ k ≤ n – 4)”.
 *
 *     T(n,k) = binomial(n, k + 2)    for  n ≥ 4, 0 ≤ k ≤ n – 4
 *     a(n) is the flattened row‑by‑row enumeration of the T‑triangle.
 * 
 * The implementation maps the linear index n to the appropriate row
 *   r  (the original n in the binomial) and column k, then returns
 *   C(r, k+2) using a multiplicative binomial routine that avoids
 *   overflow by cancelling factors with the GCD at every step.
 */

#include <stdint.h>
#include "../utils/gcd.h"   /* provides:  seq_t gcd(seq_t a, seq_t b); */

static seq_t binomial_idx(idx_t n, idx_t k)
{
    /* Symmetry to minimise k */
    if (k > n - k) k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = n - k + i;   /* numerator factor */
        seq_t den = i;           /* denominator factor */

        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        g = gcd(res, den);
        res /= g;
        den /= g;   /* den should now be 1 */

        res = res * num;           /* safe multiplication – no overflow */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Defined only for n ≥ 4.  For smaller indices return 0. */
    if (n < 4) return 0;

    /* Linear index within the flattened triangle (0‑based). */
    idx_t idx = n - 4;

    /* Find the row r such that the cumulative number of entries up to
     * that row exceeds idx.  Total entries up to and including row r:
     *   total(r) = (r - 3)(r - 2)/2   for r ≥ 4. */
    idx_t r = 4;
    for (;; ++r)
    {
        idx_t total = (r - 3) * (r - 2) / 2;
        if (total > idx) break;
    }

    /* Entries before row r. */
    idx_t before = (r - 4) * (r - 3) / 2;

    /* Position k within row r (0‑based). */
    idx_t k = idx - before;

    /* Return the binomial coefficient C(r, k+2). */
    return binomial_idx(r, k + 2);
}
