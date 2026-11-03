/*
 * A153299  G.f.: A(x) = F(x*G(x)) where F(x) = G(x*F(x)) = 1 + x*F(x)^3 is
 *          the g.f. of A001764 and G(x) = F(x/G(x)) = 1 + x*G(x)^2 is the
 *          g.f. of A000108 (Catalan).
 * https://oeis.org/A153299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} C(3k, k)/(2k+1) * C(2n-k, n-k) * k/(2n-k)
 *            (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static inline seq_t seq_one(void) { return (seq_t)1; }

seq_t a(idx_t n)
{
    /* a(0) */
    if (n == 0)
        return seq_one();

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* A_k  = C(3k, k)/(2k+1)   (the cubic Catalan numbers) */
        seq_t Ak = binomial(3 * k, k);
        Ak /= (seq_t)(2 * k + 1);

        /* B_k  = C(2n-k, n-k)     (a binomial coefficient) */
        seq_t Bk = binomial(2 * n - k, n - k);

        /* term = A_k * B_k * k/(2n-k)   (exact division) */
        seq_t term = Ak * Bk;
        term = term * (seq_t)k / (seq_t)(2 * n - k);

        sum += term;
    }

    return sum;
}
