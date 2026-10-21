/*
 * A067585  Binary representation of a(n) is obtained thus: replace every
 *          digit in the binary representation of n with "1" if the sum of its
 *          neighbors is 1 and with "0" otherwise.
 * https://oeis.org/A067585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (((n >> 1) ^ (n << 1)) & ((1 << bitlen(n)) - 1)) where
 *            bitlen(n) is the number of bits in the binary representation of
 *            n
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t x    = (seq_t)n;
    seq_t res  = (x >> 1) ^ (x << 1);

    /* compute number of bits in n */
    idx_t len  = 0;
    idx_t tmp  = n;
    while (tmp)
    {
        ++len;
        tmp >>= 1;
    }

    /* build mask of len low bits set to 1 */
    seq_t mask = 0;
    for (idx_t i = 0; i < len; ++i)
        mask = (mask << 1) | (seq_t)1;

    return res & mask;
}
