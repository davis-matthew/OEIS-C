/*
 * A213282  G.f. satisfies A(x) = G(x/(1-x)^3) where G(x) = 1 + x*G(x)^3 is
 *          the g.f. of A001764.
 * https://oeis.org/A213282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+2*k-1,n-k) *
 *            binomial(3*k,k)/(2*k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        /* term1 = binomial(n + 2*k - 1, n - k) */
        idx_t nb = n + 2 * k - 1;
        seq_t term1 = binomial(nb, n - k);
        if (term1 == 0)
            continue;

        /* term2 = binomial(3*k, k) */
        seq_t term2 = binomial(3 * k, k);

        /* denominator */
        seq_t denom = (seq_t)(2 * k + 1);

        /* cancel common factors with the denominator */
        seq_t g = gcd(term1, denom);
        term1 /= g;
        denom /= g;

        g = gcd(term2, denom);
        term2 /= g;
        denom /= g;

        /* After cancellation denom should be 1.  If not, divide at last step. */
        seq_t term = term1 * term2;
        if (denom != 0)
            term /= denom;

        sum += term;
    }

    return sum;
}
