/*
 * A183574  n+Floor[sqrt(2n+2)].
 * https://oeis.org/A183574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + Floor[sqrt(2n+2)]
 * Range:     u8 n <= 30, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t t = n_seq * ((seq_t)2) + ((seq_t)2);
    seq_t s = floor_sqrt(t);
    return n_seq + s;
}
