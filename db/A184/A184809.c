/*
 * A184809  a(n) = n + floor(sqrt(3/2)*n).
 * https://oeis.org/A184809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt(3/2) * n)
 * Range:     u8 n <= 6, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Beatty sequence for 1 + sqrt(3/2).  No floating point used. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* cast n once to avoid truncation during multiplication */
    seq_t nn  = n;
    seq_t n_sq = nn * nn;          /* n² */
    seq_t tmp   = n_sq * 3;         /* 3·n² */
    seq_t div2  = tmp / 2;          /* ⌊3·n²/2⌋ */
    seq_t r_floor = floor_sqrt(div2); /* ⌊√(3·n²/2)⌋ = ⌊√(3/2)·n⌋ */
    return nn + r_floor;
}
