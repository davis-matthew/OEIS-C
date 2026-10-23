/*
 * A140220  a(n) = binomial(n+7, 7)*5^n.
 * https://oeis.org/A140220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+7,7)*5^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 43,
 *            big any n
 * Helpers:   binomial, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    seq_t bin = binomial(n + 7, 7);   /* C(n+7,7) */
    seq_t p5  = power5(n);            /* 5^n */
    return bin * p5;                  /* product gives a(n) */
}
