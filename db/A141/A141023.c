/*
 * A141023  a(n) = 2^n - (3-(-1)^n)/2.
 * https://oeis.org/A141023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - (3-(-1)^n)/2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t val = seq_pow((seq_t)2, n);

    /* subtract (3-(-1)^n)/2, which is 1 when n is even, 2 when odd */
    if ((n & (idx_t)1) == (idx_t)0)
        val -= (seq_t)1;
    else
        val -= (seq_t)2;

    return val;
}
