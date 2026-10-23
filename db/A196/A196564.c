/*
 * A196564  Number of odd digits in decimal representation of n.
 * https://oeis.org/A196564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of odd digits in the decimal representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* not strictly needed, but harmless */

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* a(0) = 0 by definition */
    if (n == 0)
        return 0;

    while (n > 0) {
        idx_t digit = n % 10;   /* last decimal digit */
        if (digit & 1)          /* odd? (digit % 2 == 1) */
            count++;
        n /= 10;                /* drop the last digit */
    }
    return count;
}
