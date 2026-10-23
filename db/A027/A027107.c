/*
 * A027107  a(n) = Sum_{k=0..2n} (k+1) * A027082(n, 2n-k).
 * https://oeis.org/A027107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*3^(n-1) - 1 (n>0), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t exp = (seq_t)(n - 1);
    seq_t pow3 = big_pow((seq_t)3, exp);

    seq_t result = pow3 * (seq_t)7;
    return result - (seq_t)1;
}
