/*
 * A024438  Expansion of 1/((1-x)(1-7x)(1-8x)(1-10x)).
 * https://oeis.org/A024438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 26*a(n-1) – 231*a(n-2) + 766*a(n-3) – 560*a(n-4)
   Initial values: a(0)=1, a(1)=26, a(2)=445, a(3)=6330.
   This follows from 1/((1‑x)(1‑7x)(1‑8x)(1‑10x)). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;
    if (n == 2) return (seq_t)445;
    if (n == 3) return (seq_t)6330;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)26;
    seq_t a2 = (seq_t)445;
    seq_t a3 = (seq_t)6330;
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t term1 = (seq_t)26 * a3 + (seq_t)766 * a1;   /* 26*a(n-1)+766*a(n-3) */
        seq_t term2 = (seq_t)231 * a2 + (seq_t)560 * a0;   /* 231*a(n-2)+560*a(n-4) */
        next = term1 - term2;                             /* subtract the two sums   */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
