/*
 * A022529  Nexus numbers (n+1)^13-n^13.
 * https://oeis.org/A022529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^13 - n^13
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 32, u128 n <= 1312,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* compute (n+1)^13 and n^13 using 64‑bit base */
    seq_t upper = pow_uint((uint64_t)n + 1, 13);
    seq_t lower = pow_uint((uint64_t)n,     13);
    return upper - lower;
}
