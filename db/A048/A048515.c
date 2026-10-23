/*
 * A048515  a(n) = T(n,n), array T given by A048505.
 * https://oeis.org/A048515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^2 + n)*2^(n-2) - n^2 + 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 21, u64 n <= 51, u128 n <= 113,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)5;

    seq_t tn   = (seq_t)n;
    seq_t temp = (seq_t)9 * tn * tn + tn;
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n - 2));

    return temp * pow2 - tn * tn + (seq_t)1;
}
