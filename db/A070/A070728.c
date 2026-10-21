/*
 * A070728  n^7 mod 48.
 * https://oeis.org/A070728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 48
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Reduce n modulo 48 first */
    uint64_t r = (uint64_t)(n % 48U);
    /* Compute r^7 using 64‑bit arithmetic */
    uint64_t power = 1;
    for (int i = 0; i < 7; ++i)
        power *= r;
    /* Final result fits in 48, cast back to seq_t */
    return (seq_t)(power % 48U);
}
