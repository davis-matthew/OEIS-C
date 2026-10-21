/*
 * A232290  Number of n X 3 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally or antidiagonally.
 * https://oeis.org/A232290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) + 20*a(n-2) + 9*a(n-3) with initial values
 *            a(1)=7, a(2)=97, a(3)=1313
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, but harmless */
    const seq_t a1 = (seq_t)7;
    const seq_t a2 = (seq_t)97;
    const seq_t a3 = (seq_t)1313;

    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    seq_t prev2 = a1;   /* a(n-3) */
    seq_t prev1 = a2;   /* a(n-2) */
    seq_t cur   = a3;   /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = (seq_t)12 * cur + (seq_t)20 * prev1 + (seq_t)9 * prev2;
        prev2 = prev1;
        prev1 = cur;
        cur   = next;
    }

    return cur;
}
