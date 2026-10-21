/*
 * A092339  Number of adjacent identical digits in the binary representation
 *          of n.
 * https://oeis.org/A092339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(2n)=a(n)+[n even], a(2n+1)=a(n)+[n odd] */
/* Computes the number of adjacent identical bits in the binary representation of n */
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* While at least two bits remain, inspect the lowest two bits */
    while (n > 1)
    {
        seq_t low2 = n & 3;            /* bits 0 and 1 of n */
        if (low2 == 0 || low2 == 3)    /* 00 or 11 */
            count++;
        n >>= 1;                       /* shift to the next pair */
    }

    return count;
}
