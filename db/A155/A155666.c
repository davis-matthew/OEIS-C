/*
 * A155666  a(n) = 9^n - 8^n + 1.
 * https://oeis.org/A155666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n - 8^n + 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = 1, a(1) = 2, a(2) = 18, ... (OEIS A155666) */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert the index n to seq_t for use as an exponent. */
    seq_t exp = (seq_t)n;

    /* Compute 9^n and 8^n using the supplied arbitrary–precision power routine. */
    seq_t pow9 = big_pow((seq_t)9, exp);
    seq_t pow8 = big_pow((seq_t)8, exp);

    /* a(n) = 9^n - 8^n + 1. */
    seq_t result = pow9;
    result -= pow8;
    result += (seq_t)1;

    return result;
}
