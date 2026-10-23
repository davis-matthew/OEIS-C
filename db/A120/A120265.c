/*
 * A120265  a(n) = numerator(Sum_{k=1..n} 1/k!).
 * https://oeis.org/A120265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator(∑_{k=1..n} 1/k!)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Each term is reduced at every step to keep the values as small
   as possible, avoiding overflow for 64‑bit builds. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t num  = 0;   /* numerator of the current sum */
    seq_t den  = 1;   /* denominator of the current sum */
    seq_t fact = 1;   /* running factorial k! */

    for (idx_t k = 1; k <= n; ++k)
    {
        fact *= (seq_t)k;                     /* fact = k!          */

        /* Reduce before forming the new denominator: */
        seq_t g     = gcd(den, fact);         /* common part of den and fact */
        seq_t den_div = den / g;
        seq_t fact_div = fact / g;

        /* New denominator is lcm(den, fact) = den * fact_div */
        seq_t new_den = den * fact_div;

        /* New numerator: (num/den) + 1/fact = num*fact_div/den + den_div/fact */
        seq_t new_num = num * fact_div + den_div;

        /* Reduce the resulting fraction immediately */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return num;   /* numerator of the reduced sum S_n */
}
