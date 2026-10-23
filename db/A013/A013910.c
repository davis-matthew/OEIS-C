/*
 * A013910  a(n) = 24^(5*n + 1).
 * https://oeis.org/A013910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24 * (24^5)^n = 24 * 7962624^n
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    const seq_t factor = (seq_t)7962624;          /* 24^5 */
    seq_t pow_val = big_pow(factor, (seq_t)n);    /* factor^n */
    return pow_val * (seq_t)24;                  /* 24 * factor^n */
}
