/*
 * A213326  a(n) = (n+2)^n - (n+1)^n.
 * https://oeis.org/A213326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)^n - (n+1)^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base1 = (seq_t)n + 2;
    seq_t base2 = (seq_t)n + 1;
    seq_t pow1 = big_pow(base1, (seq_t)n);
    seq_t pow2 = big_pow(base2, (seq_t)n);
    return pow1 - pow2;
}
