/*
 * A131470  a(n)=smallest number that gives a product with the sum of digits
 *          of n written in base 2 greater than n.
 * https://oeis.org/A131470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 1 + floor(n / sd2(n))
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   bitcnt_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bitcnt_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    unsigned sd2 = bitcnt_uint64((uint64_t)n);
    seq_t res = (seq_t)n / sd2;
    return res + 1;
}
