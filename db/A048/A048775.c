/*
 * A048775  Number of (partially defined) monotone maps from intervals of 1..n
 *          to 1..n.
 * https://oeis.org/A048775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = binomial(2*n+1, n+1) - (n+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t res = 1;
    seq_t n_seq = (seq_t)n;

    /* Compute the binomial coefficient C(2n+1, n+1) multiplicatively,
       reducing common factors at each step to keep intermediate values
       within the limits of seq_t. */
    for (idx_t i = 1; i <= n + 1; ++i) {
        seq_t num = n_seq + (seq_t)i;   /* numerator term (n + i) */
        seq_t den = (seq_t)i;           /* denominator term i */

        /* Cancel common factors between the current result and the denominator */
        seq_t g1 = gcd(res, den);
        res = res / g1;
        den = den / g1;

        /* Cancel remaining common factors between the numerator and the denominator */
        seq_t g2 = gcd(num, den);
        num = num / g2;
        den = den / g2;  /* den should now be 1 */

        /* Multiply the reduced numerator into the result */
        res = res * num;
    }

    /* Subtract (n+1) to obtain a(n) */
    res -= (seq_t)(n + 1);
    return res;
}
