/*
 * A070617  a(n) = n^5 mod 34.
 * https://oeis.org/A070617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 34
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Work modulo 34, using 64‑bit arithmetic. */
    uint64_t m = (uint64_t)n % 34ULL;   /* Reduce the index first */
    uint64_t r = 1ULL;
    for (int i = 0; i < 5; ++i)
        r = (r * m) % 34ULL;           /* Compute m^5 mod 34 */
    return (seq_t)r;                   /* Result fits in any seq_t */
}
