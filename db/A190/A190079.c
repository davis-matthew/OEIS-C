/*
 * A190079  n + [n*s/r] + [n*t/r]; r=1, s=cos(Pi/5), t=sec(Pi/5).
 * https://oeis.org/A190079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·cos(π/5)⌋ + ⌊n·sec(π/5)⌋ Using the identities
 *            cos(π/5) = (√5+1)/4 and sec(π/5) = √5−1, a(n) simplifies to a(n)
 *            = ⌊n·√5⌋ + (n + ⌊n·√5⌋)/4
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* c = ⌊n·√5⌋  computed as floor(sqrt(5·n²)) */
    seq_t nn = (seq_t)n * (seq_t)n;
    seq_t temp = 5 * nn;
    seq_t c = floor_sqrt(temp);

    /* a(n) = c + (n + c) / 4  (integer division) */
    seq_t a_val = c + ((seq_t)n + c) / 4;
    return a_val;
}
