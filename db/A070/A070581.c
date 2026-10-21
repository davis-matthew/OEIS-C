/*
 * A070581  a(n) = n^4 mod 38.
 * https://oeis.org/A070581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 38
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Reduce n modulo 38 first to keep intermediates small. */
    uint64_t r = n % 38;
    /* Compute (r^4) mod 38 using 64‑bit arithmetic. */
    uint64_t r2 = (r * r) % 38;
    uint64_t r4 = (r2 * r2) % 38;
    return (seq_t)r4;
}
