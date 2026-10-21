/*
 * A236397  Weight of the largest-weight sunflower-free set of width n.
 * https://oeis.org/A236397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1, 2, 4, 8, 20 for n = 0…4; a(n) = n * 2^(n-2) for n >= 5
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    switch (n) {
    case 0: return (seq_t)1;
    case 1: return (seq_t)2;
    case 2: return (seq_t)4;
    case 3: return (seq_t)8;
    case 4: return (seq_t)20;
    default:
        seq_t pow = pow2(n - 2);          /* 2^(n-2) */
        return (seq_t)n * pow;            /* n * 2^(n-2) */
    }
}
