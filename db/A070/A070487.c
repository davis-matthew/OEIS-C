/*
 * A070487  a(n) = n^3 mod 25.
 * https://oeis.org/A070487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 25
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Reduce n mod 25 first to avoid overflow */
    uint64_t r = (uint64_t)n % 25ULL;
    uint64_t r2 = (r * r) % 25ULL;   /* (n^2) mod 25 */
    uint64_t r3 = (r2 * r) % 25ULL;  /* (n^3) mod 25 */
    return (seq_t)r3;
}
