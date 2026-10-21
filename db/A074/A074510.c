/*
 * A074510  a(n) = 1^n + 3^n + 8^n.
 * https://oeis.org/A074510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1^n + 3^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute 3^n and 8^n using fast exponentiation, add 1^n (=1) */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow8 = big_pow((seq_t)8, (seq_t)n);
    return pow3 + pow8 + (seq_t)1;
}
