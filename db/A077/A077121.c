/*
 * A077121  Number of integer squares <= n^3.
 * https://oeis.org/A077121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^(3/2)) + 1
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1674, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t cube = nn * nn * nn;
    seq_t root = floor_sqrt(cube);
    return root + (seq_t)1;
}
