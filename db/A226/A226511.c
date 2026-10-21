/*
 * A226511  a(n) = 3*(5^n-3^n)/2.
 * https://oeis.org/A226511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(5^n - 3^n)/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Computes A226511: a(n) = 3*(5^n - 3^n)/2. */
seq_t a(idx_t n)
{
    /* Convert the index to seq_t for the exponent argument. */
    seq_t exp = (seq_t)n;

    /* Compute 5^n and 3^n using the provided big_pow routine. */
    seq_t pow5 = big_pow((seq_t)5, exp);
    seq_t pow3 = big_pow((seq_t)3, exp);

    /* (5^n - 3^n) is always even, so divide by 2 before
       multiplying by 3 to reduce risk of overflow. */
    seq_t diff = pow5 - pow3;
    seq_t half = diff / (seq_t)2;
    seq_t result = half * (seq_t)3;

    return result;
}
