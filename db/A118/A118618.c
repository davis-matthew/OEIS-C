/*
 * A118618  Start with 1 and repeatedly reverse the digits and add 32 to get
 *          the next term.
 * https://oeis.org/A118618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n)=reverse_decimal(a(n-1))+32 for n>1 */
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n < 1, return 0 to avoid crash */

    seq_t cur = (seq_t)1;                 /* a(1) */
    for (idx_t i = 1; i < n; ++i)         /* iterate n-1 times to reach a(n) */
    {
        cur = reverse_decimal(cur) + (seq_t)32;
    }
    return cur;
}
