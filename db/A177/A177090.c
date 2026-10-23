/*
 * A177090  a(n) = a(n-1) + 12*100^(n-1), with a(0)=0.
 * https://oeis.org/A177090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(n) = a(n-1) + 12*100^(n-1) (n>=1) Recurrence:
 *            a(0)=0, a(1)=12, for n>=2: a(n) = 101*a(n-1) - 100*a(n-2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)12;

    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)12;  /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 101*a(i-1) – 100*a(i-2) */
        cur = prev1 * (seq_t)101 - prev2 * (seq_t)100;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
