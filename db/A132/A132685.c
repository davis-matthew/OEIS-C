/*
 * A132685  a(n) = binomial(2^n + 2*n, n).
 * https://oeis.org/A132685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2^n + 2*n, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */
#include "../utils/gcd.h"    /* provides seq_t gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* n = 0 -> binomial(1,0) = 1 */
    if (n == 0U)
        return 1U;

    /* N = 2^n + 2*n */
    seq_t N = pow2(n) + (seq_t)(2 * n);

    /* The numerator of the binomial is N, the lower index is n */
    seq_t res  = 1U;        /* running result of product / denominator */
    seq_t base = N - (seq_t)n;   /* N - n, used to form each numerator term */

    for (idx_t i = 1U; i <= n; ++i)
    {
        seq_t term = base + (seq_t)i;   /* N - n + i  */
        seq_t ii   = (seq_t)i;          /* copy of current denominator */

        /* cancel common factors between res and ii */
        seq_t g = gcd(res, ii);
        if (g > 1U) {
            res /= g;
            ii /= g;
        }

        /* cancel remaining factors of ii between term and ii */
        g = gcd(term, ii);
        if (g > 1U) {
            term /= g;
            ii   /= g;
        }

        /* ii should now be 1; multiply the remaining numerator part */
        res *= term;
    }
    return res;
}
