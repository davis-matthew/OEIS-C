/*
 * A071044  Number of ON cells at generation n of 1-D CA defined by Rule 22,
 *          starting with a single ON cell.
 * https://oeis.org/A071044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^popcount(n) if n is even * a(n) = (3/2) * 2^popcount(n)
 *            if n is odd
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 96 terms through n = 963 (2026-11-01)
 * Helpers:   pow2_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2_u32.h"

seq_t a(idx_t n)
{
    /* count set bits in n */
    unsigned int pop = __builtin_popcountll((uint64_t)n);

    seq_t val = pow2_u32(pop);   /* 2^popcount(n) */

    if ((n & 1) == 0)            /* even n */
        return val;
    else                         /* odd n */
        return val + val / 2;    /* val + val/2 = 3/2 * val */
}
