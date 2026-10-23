/*
 * A047938  Number of ways to label points of an n X n grid with 3 colors, up
 *          to rotational symmetry.
 * https://oeis.org/A047938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^(n^2) + 2*3^((n^2 + 3*(n mod 2))/4) + 3^((n^2 + (n mod
 *            2))/2)) / 4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t mod2 = n & 1;

    seq_t exp2 = (n2 + ((seq_t)3 * mod2)) / 4;
    seq_t exp3 = (n2 + mod2) / 2;

    seq_t val1 = big_pow((seq_t)3, n2);
    seq_t val2 = big_pow((seq_t)3, exp2);
    seq_t val3 = big_pow((seq_t)3, exp3);

    seq_t sum = val1 + val2 * ((seq_t)2) + val3;
    return sum / ((seq_t)4);
}
