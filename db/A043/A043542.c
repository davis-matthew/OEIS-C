/*
 * A043542  Number of distinct base-15 digits of n.
 * https://oeis.org/A043542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct digits in the base‑15 representation
 *            of n
 * Range:     u8 n <= 9792, u16 n <= 9792, u32 n <= 9792, u64 n <= 9792,
 *            u128 n <= 9792, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    unsigned mask = 0;
    idx_t m = n;
    while (m > 0) {
        unsigned d = (unsigned)(m % 15);
        mask |= 1u << d;
        m /= 15;
    }
    return (seq_t)__builtin_popcount(mask);
}
