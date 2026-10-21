/*
 * A091883  Expansion of (1-5x)/((1+5x)(1-10x)).
 * https://oeis.org/A091883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10^n + 2*(-5)^n)/3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = ((10^n + 2*5^n) / 3) if n is even,
/*                     ((10^n - 2*5^n) / 3) if n is odd. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow10 = big_pow((seq_t)10, (seq_t)n);
    seq_t pow5  = big_pow((seq_t)5, (seq_t)n);

    seq_t term;
    if ((n & 1) == 0)            /* even n: +2*5^n */
        term = pow10 + (pow5 * 2u);
    else                         /* odd n:  -2*5^n */
        term = pow10 - (pow5 * 2u);

    return term / 3u;
}
