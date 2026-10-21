/*
 * A221911  a(n) = 9^n + 9*n.
 * https://oeis.org/A221911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n + 9*n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t pow9 = big_pow((seq_t)9, (seq_t)n);   /* compute 9^n */
    seq_t mul9 = mul_small((seq_t)n, 9);        /* compute 9*n */
    return pow9 + mul9;
}
