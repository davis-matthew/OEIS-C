/*
 * A129533  Array read by antidiagonals: T(n,k) =
 *          binomial(n+1,2)*binomial(n+k,n+1) for 0 <= k <= n.
 * https://oeis.org/A129533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) = antidiagonal_coords(n) and T(i,j) =
 *            binomial(i+1,2) * binomial(i+j,i+1)
 * Range:     u8 n <= 38, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   antidiagonal_coords, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/antidiagonal_coords.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    seq_t part1 = binom(i + 1, 2);
    seq_t part2 = binom(i + j, i + 1);
    return part1 * part2;
}
