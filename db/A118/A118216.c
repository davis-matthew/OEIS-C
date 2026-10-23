/*
 * A118216  Start with 1 and repeatedly reverse the digits and add 69 to get
 *          the next term.
 * https://oeis.org/A118216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(1)=1, a(n)=reverse_decimal(a(n-1))+69 for n>1
 * Range:     u8 n <= 4, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    seq_t cur = 1;
    if (n <= 1) return cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = reverse_decimal(cur) + (seq_t)69;
    }
    return cur;
}
