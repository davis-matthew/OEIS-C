/*
 * A160383  Number of 3's in base-4 representation of n.
 * https://oeis.org/A160383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of digits equal to 3 in the base‑4 representation
 *            of n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0; a(4m   ) = a(4m+1) = a(4m+2) = a(m);
                a(4m+3) = a(m) + 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    while (n) {
        if ((n & 3) == 3)   /* least significant base‑4 digit is n % 4 */
            count++;
        n >>= 2;            /* divide by 4 */
    }
    return count;
}
