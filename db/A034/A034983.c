/*
 * A034983  Smallest square starting with a string of n 3's.
 * https://oeis.org/A034983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Find the smallest perfect square whose decimal representation
   begins with n copies of the digit '3' by examining successively longer
   intervals of numbers starting with that prefix. */

/* Include the helper for computing the floor of the integer square root. */
#include "../utils/floor_sqrt.h"
#include <stdint.h>

static seq_t ceil_sqrt(seq_t x)
{
    seq_t r = floor_sqrt(x);
    /* If x is 0, floor_sqrt returns 0 and r*r == x. */
    if (r * r == x)
        return r;
    return r + 1;
}

seq_t a(idx_t n)
{
    /* The smallest number with exactly n leading 3's. */
    seq_t S = 0;
    for (idx_t i = 0; i < n; ++i)
        S = S * 10 + 3;

    seq_t pow10 = 1;          /* 10^pos, starting with pos = 0 */
    for (;;)
    {
        seq_t L = S * pow10;                              /* lower bound of interval */
        seq_t U = (S + 1) * pow10 - 1;                    /* upper bound (exclusive) */

        seq_t low  = ceil_sqrt(L);
        seq_t high = floor_sqrt(U);

        if (low <= high)             /* a square exists in this interval */
            return low * low;        /* the smallest such square */

        pow10 = pow10 * 10;          /* try the next longer interval */
    }
}
