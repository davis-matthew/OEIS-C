/*
 * A169965  Numbers whose decimal expansion contains only 0's and 2's.
 * https://oeis.org/A169965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2 * Σ_{i≥0} ((n-1)>>i & 1) * 10^i
 * Range:     u8 n <= 8, u16 n <= 32, u32 n <= 1024, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. take n-1, interpret its binary digits as 0/1 in decimal, then
   replace each 1 by 2.  This yields the sequence of numbers whose
   decimal expansion consists only of 0's and 2's. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard for safety */

    idx_t m = n - 1;               /* use idx_t for bit operations */
    seq_t acc   = 0;               /* accumulation of decimal digits */
    seq_t pow10 = 1;               /* current power of 10 */

    while (m) {
        if (m & 1) acc += pow10;   /* if bit is 1, add the corresponding 10^i */
        pow10 *= 10;               /* move to next decimal place */
        m >>= 1;                   /* shift bits of m */
    }
    return acc * 2;                /* convert 1's to 2's */
}
