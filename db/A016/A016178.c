/*
 * A016178  Expansion of g.f. 1/((1 - 7x)*(1 - 9x)).
 * https://oeis.org/A016178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9^(n+1)-7^(n+1))/2 = 9*a(n-1)+7^n (a(0)=1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow9 = seq_pow((seq_t)9, n + 1);
    seq_t pow7 = seq_pow((seq_t)7, n + 1);
    return (pow9 - pow7) / (seq_t)2;
}
