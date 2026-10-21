/*
 * A088320  a(n) = 10*a(n-1) + a(n-2), starting with a(0) = 1 and a(1) = 5.
 * https://oeis.org/A088320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) + a(n-2), a(0)=1, a(1)=5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)5;   /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)10 * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
