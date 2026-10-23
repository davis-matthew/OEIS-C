/*
 * A155651  a(n) = 8^n-6^n+1^n.
 * https://oeis.org/A155651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - 6^n + 1^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow8 = big_pow((seq_t)8, (seq_t)n);
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    return pow8 - pow6 + (seq_t)1;
}
