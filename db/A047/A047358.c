/*
 * A047358  Numbers that are congruent to {2, 3} mod 7.
 * https://oeis.org/A047358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * ((n-1)/2) + 2 if n is odd; a(n) = 7 * ((n-2)/2) + 3
 *            if n is even
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) enumerates the positive integers congruent to 2 or 3 modulo 7. */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t k = (n - 1) / 2;
    if (n & 1UL) {          /* odd n */
        return (seq_t)7 * (seq_t)k + (seq_t)2;
    } else {                /* even n */
        /* k already equals (n-2)/2 because (n-1)/2 truncates */
        return (seq_t)7 * (seq_t)k + (seq_t)3;
    }
}
