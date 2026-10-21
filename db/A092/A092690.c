/*
 * A092690  Row sums of triangle A092689, which is related to the central
 *          trinomial coefficients (A002426).
 * https://oeis.org/A092690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] exp(x)*((1+x)*I0(2x)+x*I1(2x)) (OEIS A092690)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = (seq_t)1;   /* a(0) */
    seq_t T   = (seq_t)1;   /* T_0 = 1 */

    for (idx_t t = 0; t < n; ++t)
    {
        if ((t & 1) == 0)          /* t even : T_{t+1} = T_t * (n-t)/(u+1)   */
        {
            idx_t u    = t / 2;
            seq_t part = (seq_t)(n - t);                 /* numerator part  */
            seq_t denom = (seq_t)(u + 1);                /* denominator    */

            seq_t g = gcd(T, denom);
            T = T / g;
            denom = denom / g;

            seq_t g2 = gcd(part, denom);
            seq_t part1 = part / g2;
            seq_t denom2 = denom / g2;

            T = T * part1;
            if (denom2 != (seq_t)1) T = T / denom2;
        }
        else                        /* t odd : T_{t+1} = T_t * (u+2)*(n-t)/(u+1)^2   */
        {
            idx_t u = t / 2;
            seq_t num   = (seq_t)(u + 2) * (seq_t)(n - t);
            seq_t denom = (seq_t)(u + 1) * (seq_t)(u + 1);

            seq_t g1 = gcd(T, denom);
            T = T / g1;
            denom = denom / g1;

            seq_t g2 = gcd(num, denom);
            num = num / g2;
            denom = denom / g2;

            T = T * num;
            if (denom != (seq_t)1) T = T / denom;
        }

        sum += T;
    }

    return sum;
}
