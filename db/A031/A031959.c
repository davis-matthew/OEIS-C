/*
 * A031959  Numbers with exactly three distinct base-7 digits.
 * https://oeis.org/A031959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Numbers with exactly three distinct base-7 digits.
   The function scans natural numbers in order, selecting those that have
   exactly three distinct digits when written in base 7, and returns the
   nth such number (a(1) = 51). 
*/

#include <stdbool.h>

/* count distinct digits from the bit‑mask (bits 0‑6) */
static unsigned int popcnt(unsigned int mask)
{
    unsigned int c = 0;
    while (mask) {
        c++;
        mask &= mask - 1;          /* clear lowest set bit */
    }
    return c;
}

seq_t a(idx_t n)
{
    idx_t  count = 0;            /* how many valid numbers we have seen */
    seq_t  x     = 0;            /* current candidate number   */

    /* Find the n‑th number with exactly three distinct base‑7 digits */
    while (true) {
        x++;                     /* candidate must be positive        */
        seq_t  y   = x;
        unsigned int mask = 0;

        /* build the bit‑mask of digits in base 7 */
        while (y > 0) {
            unsigned int d = (unsigned int)(y % 7);
            mask |= 1u << d;
            y /= 7;
        }

        if (popcnt(mask) == 3) {
            count++;
            if (count == n) {
                return x;
            }
        }
    }
}
