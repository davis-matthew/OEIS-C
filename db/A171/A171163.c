/*
 * A171163  Number of children at height n in a doubly logarithmic tree.
 *          Leaves are at height 0.
 * https://oeis.org/A171163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2^(n-2)), a(0)=0, a(1)=2. Sequence A171163
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    seq_t two = (seq_t)2;
    seq_t exp = big_pow(two, (seq_t)(n - 2));
    return big_pow(two, exp);
}
