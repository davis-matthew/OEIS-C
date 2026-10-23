/*
 * A107232  Expansion of (1+x*c(x^2))^3/sqrt(1-4*x^2), c(x) the g.f. of
 *          A000108.
 * https://oeis.org/A107232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k)*binomial(3, n-2*k)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t N, idx_t K); */

seq_t a(idx_t n)
{
    /* binomial(3, m) values for m = 0..3 */
    static const seq_t binom3[4] = {(seq_t)1, (seq_t)3, (seq_t)3, (seq_t)1};

    seq_t result = 0;
    idx_t m;

    for (m = 0; m <= 3; ++m) {
        if (n < m)
            break;                       /* n-2k would be negative */
        /* (n - m) must be even → k integer */
        if (((n - m) & 1ULL) != 0)
            continue;
        idx_t k = (n - m) >> 1;           /* (n - m)/2 */
        seq_t term = binomial(n, k);
        term *= binom3[m];
        result += term;
    }
    return result;
}
