/*
 * A077118  Nearest integer square to n^3.
 * https://oeis.org/A077118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest square to n^3 = (round(n^(3/2)))^2
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   big_pow, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t cube = big_pow(nn, (seq_t)3);

    seq_t r     = floor_sqrt(cube);
    seq_t diff1 = cube - r * r;

    seq_t r1    = r + (seq_t)1;
    seq_t diff2 = r1 * r1 - cube;

    if (diff1 <= diff2)
        return r * r;
    else
        return r1 * r1;
}
