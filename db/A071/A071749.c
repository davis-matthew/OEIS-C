/*
 * A071749  Expansion of (1+x^4*C^2)*C^4, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4/(2*n+4) * binom(2*n+4, n) + (n>=4 ? 6/(2*n-2) *
 *            binom(2*n-2, n-4) : 0)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t res = 0;

    /* first term: coefficient of C^4 at index n */
    seq_t t1 = binom((idx_t)(2*n + 4), n);
    t1 = t1 * (seq_t)4 / (seq_t)(2*n + 4);
    res += t1;

    /* second term: coefficient of C^6 shifted by 4, only for n >= 4 */
    if (n >= 4) {
        idx_t m = n - 4;
        seq_t t2 = binom((idx_t)(2*m + 6), m);
        t2 = t2 * (seq_t)6 / (seq_t)(2*m + 6);
        res += t2;
    }

    return res;
}
