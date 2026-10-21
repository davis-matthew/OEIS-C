/*
 * A213336  G.f. satisfies A(x) = G(x/(1-x)^4) where G(x) = 1 + x*G(x)^4 is
 *          the g.f. of A002293.
 * https://oeis.org/A213336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n+3*k-1, n-k) * binom(4*k,
 *            k)/(3*k+1) (with a(0) = 1). This follows from A213336 =
 *            G(x/(1-x)^4) where G(x) = 1 + x*G(x)^4 (the 4‑ary Fuss–Catalan
 *            numbers). The implementation uses the supplied utility function
 *            binom(a,b) that returns a seq_t
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        /* compute g[k] = binom(4*k, k) / (3*k + 1) */
        seq_t gk = binom(4 * k, k) / (seq_t)(3 * k + 1);
        /* compute the binomial coefficient from the GF expansion */
        seq_t coeff = binom(n + 3 * k - 1, n - k);
        sum += gk * coeff;
    }
    return sum;
}
