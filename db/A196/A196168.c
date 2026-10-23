/*
 * A196168  In binary representation of n: replace each 0 with 1, and each 1
 *          with 10.
 * https://oeis.org/A196168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = interpret the binary digits of n after replacing each bit
 *            0 with “1” and each bit 1 with “10”, and then reading the result
 *            as a binary number. For n = 0 the binary representation is “0”,
 *            so a(0) = 1
 * Range:     u8 n <= 22, u16 n <= 382, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for n == 0 */
    if (n == 0)
        return 1;

    /* Find the position of the most significant set bit */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp >>= 1)
        ++msb;

    /* Build the result by scanning bits from most to least significant */
    seq_t res = 0;
    for (idx_t i = msb; ; --i) {
        if (n & ((idx_t)1 << i))          /* bit = 1 → append “10” */
            res = (res << 2) | 2;
        else                               /* bit = 0 → append “1” */
            res = (res << 1) | 1;

        if (i == 0) break;                /* finished all bits */
    }
    return res;
}
