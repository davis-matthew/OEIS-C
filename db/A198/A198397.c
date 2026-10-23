/*
 * A198397  a(n) = 7^n - 7*n.
 * https://oeis.org/A198397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - 7*n, a(0)=1, a(1)=0, a(2)=35
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 7^n   */
    seq_t power = big_pow((seq_t)7, (seq_t)n);
    /* 7*n   */
    seq_t sub = (seq_t)n * (seq_t)7;
    return power - sub;
}
