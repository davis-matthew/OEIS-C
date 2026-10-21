/*
 * A074309  Sum of next n terms of the form i^i.
 * https://oeis.org/A074309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=n+1..2n} i^i
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    idx_t upper = n * 2;          /* last index 2n */
    seq_t sum = 0;

    for (idx_t i = n + 1; i <= upper; ++i) {
        seq_t term = big_pow((seq_t)i, (seq_t)i); /* i^i */
        sum += term;
    }

    return sum;
}
