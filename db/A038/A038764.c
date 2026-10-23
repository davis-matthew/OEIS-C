/*
 * A038764  a(n) = (9*n^2 + 3*n + 2)/2.
 * https://oeis.org/A038764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^2 + 3*n + 2)/2
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a wider integer type for the intermediate calculation
     * to avoid overflow when seq_t is smaller than 64 bits. */
    uint64_t N = (uint64_t)n;
    uint64_t val = (uint64_t)9 * N * N + (uint64_t)3 * N + (uint64_t)2;
    return (seq_t)(val / 2);
}
