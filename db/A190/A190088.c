/*
 * A190088  Triangle of binomial coefficients binomial(3*n-k+1,3*n-3*k+1).
 * https://oeis.org/A190088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = binomial(3*r - k + 1, 3*r - 3*k + 1) where r is the
 *            unique non‑negative integer with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2,
 *            and k = n – r*(r+1)/2
 * Range:     u8 n <= 11, u16 n <= 38, u32 n <= 125, u64 n <= 418,
 *            u128 n <= 1512, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Compute the binomial coefficient C(n,k) for non-negative integers n,k.
   The computation keeps the intermediate values small by reducing
   each fraction with GCD. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        g = gcd(res, den);
        res /= g;
        den /= g;   /* den should now be 1 */

        res = res * num;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;           /* column inside the row */

    idx_t a_val = 3 * r - k + 1;
    idx_t b_val = 3 * r - 3 * k + 1;

    return binom(a_val, b_val);
}
