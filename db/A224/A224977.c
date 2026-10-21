/*
 * A224977  n^2 minus sum of digits of n^2.
 * https://oeis.org/A224977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n^2 - A004159(n^2)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    seq_t sq = ((seq_t)n) * ((seq_t)n);
    return sq - digit_sum(sq);
}
