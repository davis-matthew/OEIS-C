/*
 * A133179  A modular binomial sum transform of 2^n .
 * https://oeis.org/A133179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} mod(binomial(n,k),2) * 2^k
 * Range:     u8 n <= 23, u16 n <= 47, u32 n <= 95, u64 n <= 191,
 *            u128 n <= 383, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t half = n >> 1;            /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t s = n; ; s = (s - 1) & n)        /* iterate over all subsets of n */
    {
        if (s <= half)
            sum += pow2(s);                    /* 2^s */
        if (s == 0)
            break;
    }
    return sum;
}
