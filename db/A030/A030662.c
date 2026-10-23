/*
 * A030662  Number of combinations of n things from 1 to n at a time, with
 *          repeats allowed.
 * https://oeis.org/A030662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = binom(2*n, n) - 1 = sum_{k=0}^n binom(n,k)^2 (OEIS
 *            A030662)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute the n-th term of sequence A030662.
   The sequence is defined for n >= 1; the function returns 0 for n == 0
   to keep the offset stable. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* value outside the offset */

    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* use cast on operands before addition to avoid overflow of idx_t */
        seq_t num = (seq_t)n + (seq_t)i;   /* numerator = n + i  */
        seq_t den = (seq_t)i;             /* denominator = i   */

        /* cancel common factors between numerator and denominator       */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* cancel common factors between current result and denominator   */
        g = gcd(res, den);
        res /= g;
        den /= g;                      /* den should now be 1 */

        /* multiply by the reduced numerator (den is 1) */
        res *= num;
    }

    return res - 1;                     /* subtract 1 to get a(n)      */
}
