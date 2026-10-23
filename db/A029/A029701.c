/*
 * A029701  Number of lines on the intersection of two general hypersurfaces
 *          of degree n in complex projective (n+2)-space.
 * https://oeis.org/A029701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coeff_{x^(n+2)y^(n+1)} (x - y) * ∏_{k=0..n} (k*x +
 *            (n-k)*y)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Helper type for intermediate arithmetic.  gcc/clang support
   unsigned __int128; it is also accepted by C++17. */
typedef unsigned __int128 u128;

/* Return the n-th term of OEIS A029701. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  Return 0 for n == 0
       so that caller can treat the offset correctly. */
    if (n == 0) {
        return 0;
    }

    /* Number of factors is n+1.  The maximum possible exponent of x
       after all factors is 2*(n+1), but we only need coefficients up
       to x^(n+2).  Allocate one extra slot to avoid bounds checks
       inside the loop. */
    size_t sz = (size_t)n + 3;
    u128 *dp   = (u128 *)malloc(sz * sizeof(u128));
    u128 *next = (u128 *)malloc(sz * sizeof(u128));
    if (!dp || !next) {   /* allocation failed */
        free(dp);
        free(next);
        return 0;
    }

    /* init dp[0] = 1, others 0 */
    memset(dp, 0, sz * sizeof(u128));
    dp[0] = 1;

    /* Process each factor (k*x + (n-k)*y)^2 for k = 0 .. n */
    for (idx_t k = 0; k <= n; ++k) {
        unsigned long long kk = (unsigned long long)k;
        unsigned long long nk = (unsigned long long)n - kk;

        unsigned long long a0 = nk * nk;          /* coefficient of y^2     */
        unsigned long long a1 = 2ULL * kk * nk;   /* coefficient of xy      */
        unsigned long long a2 = kk * kk;          /* coefficient of x^2     */

        /* zero next array */
        memset(next, 0, sz * sizeof(u128));

        /* convolution step: only previous exponents up to 2*k are non‑zero */
        size_t max_prev = 2 * (size_t)k;
        for (size_t i = 0; i <= max_prev; ++i) {
            u128 cur = dp[i];
            if (cur == 0) continue;

            if (i + 2 < sz) {
                next[i + 2] += cur * a2;
            }
            if (i + 1 < sz) {
                next[i + 1] += cur * a1;
            }
            if (i < sz) {
                next[i] += cur * a0;
            }
        }

        /* swap dp and next for next iteration */
        u128 *tmp = dp;
        dp = next;
        next = tmp;
    }

    /* After all factors, the coefficient we need is:
       a(n) = coeff_{x^(n+1)y^(n+1)} P - coeff_{x^(n+2)y^n} P
       where P = ∏(k*x + (n-k)*y)^2.  */
    u128 coeff1 = dp[(size_t)n + 1];
    u128 coeff2 = dp[(size_t)n + 2];
    u128 res128 = coeff1 - coeff2;

    /* Convert to return type (may truncate if the value does not
       fit in seq_t; that's acceptable for builds where seq_t is
       smaller than the true value). */
    seq_t result = (seq_t)res128;

    free(dp);
    free(next);
    return result;
}
