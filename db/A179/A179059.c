/*
 * A179059  Number of non-attacking placements of 4 rooks on an n X n board.
 * https://oeis.org/A179059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 24 * binomial(n,4)^2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 25, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    uint64_t nn = (uint64_t)n;
    __int128 prod = (__int128)nn * (nn - 1) * (nn - 2) * (nn - 3);
    uint64_t bin = (uint64_t)(prod / 24);          /* C(n,4) */
    __int128 result = (__int128)24 * ( (__int128)bin * bin ); /* 24 * C(n,4)^2 */
    return (seq_t)result;
}
