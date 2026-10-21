/*
 * A097255  Numbers whose set of base 9 digits is {0,8}.
 * https://oeis.org/A097255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 8*A033046(n) Recurrence: a(0) = 0 a(2n) = 9*a(n) a(2n+1)
 *            = 9*a(n) + 8 The algorithm processes the bits of n from most
 *            significant to least, updating an accumulator that represents
 *            the base‑9 number whose digits are 0 (for bit 0) or 8 (for bit
 *            1)
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   msb_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/msb_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Find the highest bit set in n. */
    idx_t msb = msb_idx(n);
    idx_t shift = (idx_t)1 << msb;  /* 2^msb */

    seq_t result = (seq_t)0;

    /* Iterate from the most significant bit down to 0. */
    while (shift) {
        result = result * (seq_t)9;          /* shift base‑9 left */
        if (n & shift)
            result = result + (seq_t)8;     /* append digit 8 for bit 1 */
        shift >>= 1;
    }

    return result;
}
