/*
 * A104571  Triangle T(n,k) = A042948(n-k+1) read by rows, 0<=k<=n.
 * https://oeis.org/A104571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2*d + (d%2 ? 2 : 1) where d = row - column, row =
 *            ⌊(√(8*n+1)-1)/2⌋, column = n - row(row+1)/2
 * Range:     u8 n <= 35, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   isqrt_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_uint128.h"

static idx_t row_of(idx_t n)
{
    // row = floor((sqrt(8*n+1)-1)/2)
    unsigned __int128 t = (unsigned __int128)8 * n + 1;
    idx_t sqrt_floor = isqrt_uint128(t);
    return (sqrt_floor - 1) >> 1;
}

seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
    idx_t off = n - (idx_t)tri;
    idx_t d = r - off;
    seq_t val = ((seq_t)d) * 2 + ((d & 1) ? 2 : 1);
    return val;
}
