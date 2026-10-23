/*
 * A036442  a(n) = 2^((n-1)*(n+2)/2).
 * https://oeis.org/A036442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^((n-1)*(n+2)/2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* use seq_t to avoid overflow when n is small */
    seq_t N = (seq_t)(n - 1);
    seq_t M = (seq_t)(n + 2);
    seq_t prod = N * M;          /* (n-1)*(n+2)  */
    seq_t exp  = prod / (seq_t)2;
    return pow2((idx_t)exp);     /* 2^exp */
}
