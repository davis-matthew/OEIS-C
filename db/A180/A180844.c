/*
 * A180844  a(n) = (27^n - 2^n)/25.
 * https://oeis.org/A180844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (27^n - 2^n)/25 Recurrence: a(0)=0,
 *            a(n)=27*a(n-1)+2^(n-1) for n≥1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t prev = 0;      /* a(n-1)   */
    seq_t pow2 = 1;      /* 2^(n-1) at the start of the loop */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t cur = mul_small(prev, 27) + pow2;   /* a(i) = 27*a(i-1)+2^(i-1) */
        prev = cur;
        pow2 = mul_small(pow2, 2);                /* advance 2^(i) */
    }

    return prev;   /* now prev == a(n) */
}
