/*
 * A096947  Tenth column of (1,5)-Pascal triangle A096940.
 * https://oeis.org/A096947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 45) * binomial(n + 8, 8) / 9
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute C(n+8,8) */
    seq_t binom = binomial(n + 8, 8);

    /* split the numerator and denominator to avoid overflow */
    seq_t num = (seq_t)(n + 45);                /* (n + 45)          */
    seq_t g   = gcd(num, (seq_t)9);             /* gcd( n+45, 9 )   */
    seq_t num_div = num / g;                    /* (n+45)/g         */
    seq_t denom   = 9 / g;                      /* 9/g              */
    seq_t binom_div = binom / denom;            /* C(n+8,8)/denom   */

    return num_div * binom_div;
}
