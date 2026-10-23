/*
 * A196666  Expansion of g.f. (1 - 7*x)/(1 - 22*x).
 * https://oeis.org/A196666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0, a(n)=15*22^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    /* n>=1: compute 15 * 22^(n-1) */
    seq_t power = seq_pow((seq_t)22, n - 1);
    return (seq_t)15 * power;
}
