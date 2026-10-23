/*
 * A165618  a(n) = binomial(n+8,8) - 1.
 * https://oeis.org/A165618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+8,8) - 1
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 55, u64 n <= 959,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute C(n+8,8) multiplicatively using 128‑bit arithmetic
     * to avoid overflow until the final result.  The intermediate
     * value fits comfortably for all n < 10000 (approx 1e32 < 2^107). */
    unsigned __int128 r = 1;
    for (idx_t i = 1; i <= 8; ++i) {
        r = r * (unsigned __int128)(n + i);
        r /= i;
    }

    /* Convert to seq_t and subtract 1.  The subtraction is valid
     * because the value fits into seq_t whenever we are called. */
    return (seq_t)r - (seq_t)1;
}
