/*
 * A170940  a(n) = 4^n - 2^n - 2.
 * https://oeis.org/A170940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 2^n - 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    return pow4 - pow2 - (seq_t)2;
}
