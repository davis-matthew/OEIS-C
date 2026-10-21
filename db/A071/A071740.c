/*
 * A071740  Expansion of (1+x^3*C^4)*C^3, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n+1, n+1) - binomial(2*n, n-2) - binomial(2*n,
 *            n-4)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* handle the small cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)29;

    /* for n >= 4 the closed‑form applies */
    idx_t k1 = n + 1;            /* n+1 >= 5 */
    idx_t k2 = n >= 2 ? n - 2 : 0;   /* safe for all n>=4 */
    idx_t k3 = n >= 4 ? n - 4 : 0;   /* safe for all n>=4 */

    seq_t term1 = binomial(2*n + 1, k1);
    seq_t term2 = binomial(2*n, k2);
    seq_t term3 = binomial(2*n, k3);

    return term1 - term2 - term3;
}
