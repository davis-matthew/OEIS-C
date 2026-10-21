/*
 * A054238  Array read by downward antidiagonals: T(i,j) = bits of binary
 *          expansion of i interleaved with that of j.
 * https://oeis.org/A054238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base, tabl
 * Formula:   a(n) = T(i, j) where i = n - m*(m+1)/2, j = m - i and m =
 *            floor((sqrt(8*n+1)-1)/2) is the antidiagonal number. T(i, j)
 *            interleaves the binary bits of j into the even positions and the
 *            bits of i into the odd positions of the result
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   interleave_bits, max_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/interleave_bits.h"
#include "../utils/max_m.h"

seq_t a(idx_t n)
{
    /* Locate antidiagonal m. */
    idx_t m = max_m(n);

    /* Offset of first element of diagonal m. */
    unsigned __int128 off128 = (unsigned __int128)m * (m + 1) / 2;
    idx_t     offset       = (idx_t)off128;          /* fits because off128 <= n */

    /* Position within the diagonal. */
    idx_t i = n - offset;         /* 0 <= i <= m */
    idx_t j = m - i;

    return interleave_bits(i, j);
}
