/*
 * A198962  a(n) = 5*9^n-1.
 * https://oeis.org/A198962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A198962
   Formula: a(n) = 5*9^n - 1
   Recurrence: a(0)=4, a(n)=9*a(n-1)+8 (n>0)
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t power = 1;          /* 9^0 */
    seq_t base  = 9;
    idx_t e = n;

    while (e) {
        if (e & 1)
            power *= base;    /* multiply by current power of 9 */
        base *= base;         /* square the base for next bit */
        e >>= 1;              /* move to the next bit of n */
    }

    power *= 5;               /* 5*9^n */
    power -= 1;               /* subtract 1 */
    return power;
}
