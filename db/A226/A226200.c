/*
 * A226200  a(n) = 6^n + n.
 * https://oeis.org/A226200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n + n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 6^n */
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    /* 6^n + n */
    return pow6 + (seq_t)n;
}
