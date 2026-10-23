/*
 * A028052  Expansion of 1/((1-3x)(1-4x)(1-11x)(1-12x)).
 * https://oeis.org/A028052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7*12^(n+3)-9*11^(n+3)+9*4^(n+3)-7*3^(n+3))/504
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=30, a(2)=595, a(3)=9900 */
/* a(n) = 30*a(n-1) - 305*a(n-2) + 1200*a(n-3) - 1584*a(n-4) for n>=4. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)30;
    if (n == 2) return (seq_t)595;
    if (n == 3) return (seq_t)9900;

    /* a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a0 = (seq_t)1;   /* a(0)   */
    seq_t a1 = (seq_t)30;  /* a(1)   */
    seq_t a2 = (seq_t)595; /* a(2)   */
    seq_t a3 = (seq_t)9900;/* a(3)   */

    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)30 * a3 - (seq_t)305 * a2 + (seq_t)1200 * a1 - (seq_t)1584 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
