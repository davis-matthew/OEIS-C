/*
 * A083223  a(n) = (5*6^n+(-6)^n)/6.
 * https://oeis.org/A083223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5*6^n+(-6)^n)/6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   _pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_pow_seq_t.h"

seq_t a(idx_t n)
{
    /* 6^n */
    seq_t pow6 = _pow_seq_t((seq_t)6, n);

    /* n = 0  → a(0) = 6^0 = 1 */
    if (n == (idx_t)0)
        return pow6;

    /* Even n : a(n) = 6^n
       Odd  n : a(n) = 4 * 6^{n-1}            */
    if ((n & (idx_t)1) == 0)
        return pow6;

    seq_t pow6prev = _pow_seq_t((seq_t)6, n - (idx_t)1);
    return (seq_t)4 * pow6prev;
}
