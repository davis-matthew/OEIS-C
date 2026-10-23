/*
 * A011844  a(n) = floor(binomial(n,7)/8).
 * https://oeis.org/A011844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(binomial(n,7)/8)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 110, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 7)
        return 0;
    seq_t b = binomial(n, 7);
    return b / (seq_t)8;
}
