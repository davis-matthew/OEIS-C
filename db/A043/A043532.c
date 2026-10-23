/*
 * A043532  Number of distinct base-5 digits of n.
 * https://oeis.org/A043532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct digits in the base‑5 representation of
 *            n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned mask = 0;
    idx_t x = n;

    /* Determine which base‑5 digits appear */
    while (x) {
        unsigned d = x % 5;
        mask |= 1U << d;
        x /= 5;
    }

    /* Count distinct digits */
    unsigned count = 0;
    while (mask) {
        count++;
        mask &= mask - 1;
    }

    return (seq_t)count;
}
