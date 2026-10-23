/*
 * A155673  a(n) = 11^n + 10^n - 1.
 * https://oeis.org/A155673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n + 10^n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    const seq_t base11 = (seq_t)11;
    const seq_t base10 = (seq_t)10;

    seq_t pow11 = big_pow(base11, (seq_t)n);
    seq_t pow10 = big_pow(base10, (seq_t)n);
    seq_t one   = (seq_t)1;

    return pow11 + pow10 - one;
}
