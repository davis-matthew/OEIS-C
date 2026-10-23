/*
 * A129290  Base-3 Fermat numbers: a(n) = 3^(3^n) + 1.
 * https://oeis.org/A129290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(3^n) + 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   big_pow, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"    /* seq_t pow3(idx_t exp) */
#include "../utils/big_pow.h" /* seq_t big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* Compute the exponent 3^n */
    seq_t exp = pow3(n);

    /* Compute 3^(3^n) */
    seq_t val = big_pow((seq_t)3, exp);

    /* Add 1 to get the sequence term */
    return val + 1;
}
