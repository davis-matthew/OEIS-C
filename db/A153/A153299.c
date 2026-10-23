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
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    /* a(0) */
    if (n == 0)
        return seq_one();

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* A_k  = C(3k, k)/(2k+1)   (the cubic Catalan numbers) */
        seq_t Ak = binom(3 * k, k);
        Ak /= (seq_t)(2 * k + 1);

        /* B_k  = C(2n-k, n-k)     (a binomial coefficient) */
        seq_t Bk = binom(2 * n - k, n - k);

        /* term = A_k * B_k * k/(2n-k)   (exact division) */
        seq_t term = Ak * Bk;
        term = term * (seq_t)k / (seq_t)(2 * n - k);

        sum += term;
    }

    return sum;
}
