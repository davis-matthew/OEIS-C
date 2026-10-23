/*
 * A016184  Expansion of 1/((1-7*x)*(1-12*x)).
 * https://oeis.org/A016184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A016184:  a(n) = (12^(n+1) - 7^(n+1)) / 5 */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* exponent (n+1) – scalar as seq_t */
    const seq_t exp = (seq_t)(n + 1);

    /* compute the powers */
    seq_t p12 = big_pow((seq_t)12, exp);
    seq_t p7  = big_pow((seq_t)7,  exp);

    /* a(n) = (12^(n+1) - 7^(n+1)) / 5 */
    return (p12 - p7) / (seq_t)5;
}
