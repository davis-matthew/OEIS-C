/*
 * A099125  Number of orbits of the wreath product of S_n with S_n on n X n
 *          matrices over {0,1,2,3,4,5,6}.
 * https://oeis.org/A099125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(binomial(n + 6, n) + n - 1, n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* first binomial: C(n+6,6) */
    seq_t m = binom(n + 6, (idx_t)6);

    /* X = m + n - 1 */
    seq_t X = m + (seq_t)n - (seq_t)1;

    /* compute C(X, n) multiplicatively */
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t num = X - (seq_t)(n - i);
        seq_t g   = gcd(num, (seq_t)i);
        num /= g;
        seq_t den = (seq_t)i / g;

        res *= num;
        if (den != (seq_t)1) {
            res /= den;
        }
    }
    return res;
}
