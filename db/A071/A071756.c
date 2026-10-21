/*
 * A071756  Expansion of (1+x^4*C^4)*C^3, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] (1 + x^4*C^4)*C^3 where C = (1 - (1-4x)^{1/2})/(2x)
 *            is the Catalan GF. The coefficient of x^n is Fuss-Catalan(3,n) +
 *            (n≥4?Fuss-Catalan(7,n-4):0), where Fuss-Catalan(k,n) = k/(2*n+k)
 *            * binom(2*n+k , n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

static seq_t fuss_k(idx_t n, idx_t k)
{
    seq_t v = binom(2 * n + k, n);        /* binom(2n+k, n) */
    v *= (seq_t)k;                        /* multiply by k      */
    v /= (seq_t)(2 * n + k);              /* divide by (2n+k)   */
    return v;
}

seq_t a(idx_t n)
{
    seq_t res = fuss_k(n, 3);              /* coefficient from C^3      */
    if (n >= 4)
        res += fuss_k(n - 4, 7);          /* coefficient from x^4*C^7   */
    return res;
}
