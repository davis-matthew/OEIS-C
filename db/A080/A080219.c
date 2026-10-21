/*
 * A080219  Decimal expansion of exponential factorial constant Sum_{n>=1}
 *          1/A049384(n).
 * https://oeis.org/A080219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A080219
   Digit d (index n) of the decimal expansion of the exponential
   factorial constant S = Σ_{k≥1} 1 / a(k)
   where a(1)=1 and a(k)=k^{a(k-1)} (exponential factorial).
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Pre‑compute the first four exponential factorials.
       a1 = 1
       a2 = 2
       a3 = 3^2     = 9
       a4 = 4^9     = 262144
       Subsequent terms are astronomically large and are
       irrelevant for the first many decimal places.       */
    static const seq_t terms[5] = {0, 1, 2, 9, 262144};

    /* Sum 1/a(k) for k=1..4 as a rational number num/den. */
    seq_t num = 0, den = 1;
    for (size_t i = 1; i < 5; ++i) {
        num = num * terms[i] + den;   /* num = num/den + 1/terms[i]  */
        den = den * terms[i];         /* new denominator            */
    }

    /* n == 1 : return the integer part of the constant. */
    if (n == 1u)
        return num / den;

    /* For n >= 2 : return the (n‑1)-th decimal digit. */
    idx_t pos = n - 1;                 /* zero‑based decimal position */
    seq_t rem  = num % den;            /* remainder after integer part */
    seq_t digit = 0;

    /* Long division to extract the required digit without using
       arbitrarily huge powers of ten. */
    for (idx_t i = 0; i < pos; ++i) {
        rem   *= 10;
        digit  = rem / den;          /* current decimal digit      */
        rem   %= den;                /* remainder for next step    */
    }
    return digit;
}
