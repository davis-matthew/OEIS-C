/*
 * A086013  Number of 5's in decimal expansion of n^2.
 * https://oeis.org/A086013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of digit '5' in the decimal representation of n^2
 * Range:     u8 n <= 15, u16 n <= 260, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_fives
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/count_fives.h"

/* OEIS sequence A086013: number of 5's in decimal expansion of n^2 */
seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;   /* n^2 */
    return count_fives(sq);
}
