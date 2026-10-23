/*
 * A199419  a(n) = 4*7^n+1.
 * https://oeis.org/A199419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*7^n+1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);
    return 4 * pow7 + 1;
}
