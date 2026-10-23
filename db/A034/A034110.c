/*
 * A034110  Fractional part of square root of a(n) starts with 4: first term
 *          of runs.
 * https://oeis.org/A034110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n^2 + ⌊(20 n + 4)/25⌋ + 1 (first term of each run of
 *            integers whose fractional part of √(k) starts with 4)
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for stdint types used by seq_t/idx_t */

seq_t a(idx_t n)
{
    /* Ensure operations use seq_t */
    seq_t s = (seq_t)n;

    /* part1 = s^2 */
    seq_t part1 = s * s;

    /* part2 = ⌊(20s + 4)/25⌋ */
    seq_t part2 = ( ( (seq_t)20 * s ) + (seq_t)4 ) / (seq_t)25;

    /* result */
    return part1 + part2 + (seq_t)1;
}
