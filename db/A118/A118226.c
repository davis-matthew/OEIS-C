/*
 * A118226  Start with 1 and repeatedly reverse the digits and add 76 to get
 *          the next term.
 * https://oeis.org/A118226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(1)=1; for n>1, a(n)=reverse_decimal(a(n-1))+76
 * Range:     u8 n <= 3, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;

    seq_t cur = (seq_t)1;           /* a(1) */
    if (n == (idx_t)1)
        return cur;

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        cur = reverse_decimal(cur) + (seq_t)76;
    }
    return cur;
}
