/*
 * A199025  a(n) = (7*11^n-1)/2.
 * https://oeis.org/A199025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*11^n-1)/2. Equivalent recurrence: a(0) = 3, a(n) =
 *            11*a(n-1) + 5
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = 3;                     /* a(0) */
    if (n == 0)
        return val;

    /* Compute a(n) iteratively using the recurrence. */
    for (idx_t i = 1; i <= n; ++i) {
        val = val * (seq_t)11 + (seq_t)5;
    }
    return val;
}
