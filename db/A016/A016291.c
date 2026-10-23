/*
 * A016291  Expansion of 1/((1-2x)*(1-4x)*(1-9x)).
 * https://oeis.org/A016291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2/7)*2^n - (8/5)*4^n + (81/35)*9^n Recurrence: a(0)=1,
 *            a(1)=15, a(2)=163, a(n)=15*a(n-1)-62*a(n-2)+72*a(n-3) for n>=3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)15;
    if (n == 2) return (seq_t)163;

    seq_t a0 = (seq_t)1;   /* a(n-3)  */
    seq_t a1 = (seq_t)15;  /* a(n-2)  */
    seq_t a2 = (seq_t)163; /* a(n-1)  */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t curr = (seq_t)15 * a2
                   - (seq_t)62 * a1
                   + (seq_t)72 * a0;
        a0 = a1;
        a1 = a2;
        a2 = curr;
    }
    return a2;
}
