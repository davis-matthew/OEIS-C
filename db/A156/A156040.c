/*
 * A156040  Number of compositions (ordered partitions) of n into 3 parts
 *          (some of which may be zero), where the first is at least as great
 *          as each of the others.
 * https://oeis.org/A156040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor((n^2 + 4*n)/6)
 * Range:     u8 n <= 37, u16 n <= 625, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    /* compute n*(n+4)/6 in 64‑bit to avoid overflow, then cast back */
    uint64_t val = 1 + (nn * (nn + 4U)) / 6U;
    return (seq_t)val;
}
