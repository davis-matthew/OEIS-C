/*
 * A094977  a(n) = floor(10^n/3^n).
 * https://oeis.org/A094977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(10^n / 3^n)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t a_val = 1;   /* a(0) */
    seq_t rem   = 0;   /* remainder 10^k mod 3^k */
    seq_t den   = 1;   /* 3^k */

    for (idx_t i = 0; i < n; ++i)
    {
        /* split a_val into quotient and remainder of division by 3 */
        seq_t a_div3 = a_val / 3;
        seq_t a_mod3 = a_val % 3;

        /* floor( (10 * a_val) / 3 )  without overflow */
        seq_t q = a_div3 * 10 + (a_mod3 * 10) / 3;

        /* carry is the integer part of ( (a_mod3 * den) + rem * 10 ) / (3 * den) */
        seq_t carry_num = (a_mod3 * den) + (rem * 10);
        seq_t carry_den = 3 * den;
        seq_t carry = carry_num / carry_den;

        /* update the sequence value */
        a_val = q + carry;

        /* update the remainder: rem' = rem * 10 + a_mod3 * den - carry * 3 * den */
        seq_t tmp = (rem * 10) + (a_mod3 * den);
        rem = tmp - (carry * 3 * den);

        /* next denominator value */
        den = den * 3;
    }

    return a_val;
}
