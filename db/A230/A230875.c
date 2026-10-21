/*
 * A230875  a(1)=0; thereafter a(n) = 2^a(ceiling(n/2)).
 * https://oeis.org/A230875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n>1, a(n) = 2^a(ceil(n/2))
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    seq_t exp = a((n + 1) / 2);
    return big_pow((seq_t)2, exp);
}
