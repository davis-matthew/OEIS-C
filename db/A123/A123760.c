/*
 * A123760  Numbers whose binary expansion is 1xy100...0 where x,y = 0 or 1.
 * https://oeis.org/A123760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (9 + 2*((n-1)%4)) * 2^((n-1)/4)
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 116, u64 n <= 244,
 *            u128 n <= 500, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute block index k and offset within each block of 4 terms. */
    idx_t k   = (n - 1) / 4;        /* ⌊(n-1)/4⌋  */
    idx_t off = (n - 1) % 4;        /* (n-1) mod 4 */

    /* Base value 2^k.  pow2(k) returns a seq_t. */
    seq_t base = pow2(k);

    /* Pattern value in {9,11,13,15}.  Add the offset scaled by 2. */
    seq_t pattern = (seq_t)(9 + 2 * off);

    /* Result: (9 + 2*off) * 2^k. */
    return base * pattern;
}
