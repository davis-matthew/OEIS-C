/*
 * A107421  a(n) = binomial(n+6,6)*binomial(n+9,9).
 * https://oeis.org/A107421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+6,6) * binomial(n+9,9)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 65, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t part1 = binomial(n + 6, 6);
    seq_t part2 = binomial(n + 9, 9);
    return part1 * part2;
}
