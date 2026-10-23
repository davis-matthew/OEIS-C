/*
 * A183159  The number of partial isometries (of an n-chain) of fix zero (fix
 *          of alpha = 0). Equivalently, the number of partial derangement
 *          isometries (of an n-chain).
 * https://oeis.org/A183159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((51 + (-1)^n)*2^n - 40)/12 - n*(n+3)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^n as an arbitrary-precision or machine integer */
    seq_t t = pow2(n);

    /* (51 + (-1)^n) is 52 for even n, 50 for odd n */
    seq_t term = ((n & 1ULL) == 0) ? (seq_t)52 * t : (seq_t)50 * t;

    /* ((51+(-1)^n)*2^n - 40)/12 */
    seq_t part1 = (term - (seq_t)40) / (seq_t)12;

    /* n*(n+3) as a seq_t */
    seq_t part2 = (seq_t)n * ((seq_t)n + (seq_t)3);

    return part1 - part2;
}
