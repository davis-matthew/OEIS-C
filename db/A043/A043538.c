/*
 * A043538  Number of distinct base-11 digits of n.
 * https://oeis.org/A043538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct base-11 digits of n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned int mask = 0;                 /* bitmask for digits 0–10 */
    idx_t tmp = n;

    while (tmp != 0) {
        unsigned int digit = tmp % 11u;     /* current base‑11 digit */
        mask |= 1u << digit;               /* record the digit */
        tmp /= 11u;                        /* move to next digit */
    }

    seq_t count = 0;
    while (mask != 0) {
        count++;                           /* count set bits in mask */
        mask &= mask - 1;                  /* clear lowest set bit */
    }
    return count;
}
