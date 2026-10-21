/*
 * A070496  a(n) = n^3 mod 34.
 * https://oeis.org/A070496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 34
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* reduce n modulo 34 first – all intermediate values are ≤33 */
    uint64_t r  = n % 34ULL;
    uint64_t r2 = (r * r) % 34ULL;
    uint64_t r3 = (r2 * r) % 34ULL;
    return (seq_t)r3;
}
