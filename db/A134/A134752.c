/*
 * A134752  a(n) = 3^(2*n-1) + 2.
 * https://oeis.org/A134752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(2*n-1)+2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t exp = 2 * (seq_t)n - 1;
    seq_t val = big_pow((seq_t)3, exp);
    return val + 2;
}
