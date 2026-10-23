/*
 * A188511  a(n) = floor(7*n/10).
 * https://oeis.org/A188511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(7*n/10)
 * Range:     u8 n <= 365, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t prod = (uint64_t)n * 7;
    return (seq_t)(prod / 10);
}
