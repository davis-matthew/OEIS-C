/*
 * A143769  Expansion of 3*x*(3*x+1)*(2*x-1) / ( (1+x)*(3*x^2+1) ).
 * https://oeis.org/A143769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (3/2) * ((2*(-1)^n - 3) * (-3)^⌊n/2⌋ - 3*(-1)^n)
 * Range:     i8 n <= 6, i16 n <= 16, i32 n <= 36, i64 n <= 76, i128 n <= 158,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      n ≥ 1; a(1) = -3, a(2) = 0, a(3) = 27, ... */

#include <stdint.h>
#include "../utils/big_pow.h"   /* defines seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but harmless */

    /* (-1)^n as a signed seq_t value */
    seq_t sign_n = (n & 1) ? (seq_t)-1 : (seq_t)1;

    /* floor(n/2) */
    idx_t half_exp = n / 2;

    /* (-3)^⌊n/2⌋ using arbitrary precision if needed */
    seq_t pow_neg3 = big_pow((seq_t)-3, (seq_t)half_exp);

    /* (2*(-1)^n - 3) */
    seq_t term1 = 2 * sign_n - (seq_t)3;

    /* ((2*(-1)^n - 3) * (-3)^⌊n/2⌋) */
    seq_t term2 = term1 * pow_neg3;

    /* 3*(-1)^n */
    seq_t term3 = 3 * sign_n;

    /* expression inside the parentheses */
    seq_t expr = term2 - term3;    /* guaranteed even */

    /* (3/2) * expr  =>  (expr/2) * 3   */
    seq_t half = expr / 2;
    return half * (seq_t)3;
}
