/*
 * A021971  Decimal expansion of 1/967.
 * https://oeis.org/A021971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 967) mod 10
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t r = mod_pow((seq_t)10, n, (seq_t)967);
    return (r * (seq_t)10) / (seq_t)967;
}
