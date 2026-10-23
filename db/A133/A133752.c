/*
 * A133752  a(n) = 256^n.
 * https://oeis.org/A133752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 256^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;          /* 256^0 = 1 */
    seq_t base   = (seq_t)256;        /* the base */

    while (n > 0)
    {
        if (n & 1)
            result *= base;
        base *= base;
        n >>= 1;
    }
    return result;
}
