/*
 * A034269  a(n) = f(n,n+2) where f is given in A034261.
 * https://oeis.org/A034269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) =
 *            4^(n+1)*Pi^(-1/2)*Gamma(n+3/2)/Gamma(n+5)*(n+1)*n*(n^2+3*n+1) *
 *            The function below evaluates a(n) for n ≥ 0 using a
 *            multiplicative recurrence * derived from the closed form, which
 *            guarantees exact integer arithmetic
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t ans = 1;              /* a(1) */
    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t kk = (seq_t)k;
        seq_t k1 = kk + 1;                           /* n+1 */
        seq_t k2 = kk * kk + 3 * kk + 1;             /* n^2 + 3n + 1 */
        seq_t two = 2;
        seq_t b = two * k1 * k2 * (2 * kk + 1);      /* numerator before reduction */
        seq_t d = (kk + 4) * (kk - 1) * (kk * kk + kk - 1); /* denominator */

        /* Reduce b and d by their gcd */
        seq_t g1 = seq_gcd(b, d);
        b /= g1; d /= g1;

        /* Remove common factors of ans and d */
        seq_t g2 = seq_gcd(ans, d);
        ans /= g2; d /= g2;

        /* Remove remaining factors of d from b */
        seq_t g3 = seq_gcd(b, d);
        b /= g3; d /= g3;

        /* After reductions, d must be 1 */
        ans = ans * b;
    }
    return ans;
}
