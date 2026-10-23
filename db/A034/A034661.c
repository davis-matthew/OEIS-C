/*
 * A034661  Sum of n-th powers of divisors of 18.
 * https://oeis.org/A034661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n + 1) * (3^n + 9^n + 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t two_n = big_pow((seq_t)2, (seq_t)n);
    seq_t three_n = big_pow((seq_t)3, (seq_t)n);
    seq_t nine_n = big_pow((seq_t)9, (seq_t)n);

    seq_t part1 = two_n + (seq_t)1;
    seq_t part2 = three_n + nine_n + (seq_t)1;

    return part1 * part2;
}
