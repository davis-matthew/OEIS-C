/*
 * A192479  a(n) = 2^n*C(n-1) - A186997(n-1), where C(n) are the Catalan
 *          numbers (A000108).
 * https://oeis.org/A192479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=ceil(n/2)}^n binomial(k, n-k) * binomial(n+2*k-2,
 *            k) / (n+k-1) where n≥1. The binom() helper from utils/binom.h
 *            returns the binomial coefficient C(a,b)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term. a(0) is undefined. */
    if (n == 0) return 0;           /* not used when n≥1 */

    idx_t start = (n + 1) / 2;      /* ceil(n/2) */
    seq_t sum = 0;

    for (idx_t k = start; k <= n; ++k) {
        idx_t r   = n - k;                 /* n-k */
        seq_t t1  = binom(k, r);            /* C(k, n-k)   */
        seq_t t2  = binom(n + 2 * k - 2, k);/* C(n+2k-2, k)*/
        seq_t den = (seq_t)(n + k - 1);     /* n+k-1       */
        seq_t term = (t1 * t2) / den;       /* exact division */
        sum += term;
    }
    return sum;
}
