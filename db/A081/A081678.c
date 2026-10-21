/*
 * A081678  a(n) = (4*6^n - 3*5^n - 3^n)/6.
 * https://oeis.org/A081678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence from OEIS A081678
   Recurrence: a(0)=0, a(1)=1, a(2)=10
                a(n) = 14*a(n-1) - 63*a(n-2) + 90*a(n-3)   (n ≥ 3)   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle the three known base terms */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)10;

    /* iterate the recurrence.  Only three previous values are needed. */
    seq_t a0 = (seq_t)0;      /* a(n-3)   */
    seq_t a1 = (seq_t)1;      /* a(n-2)   */
    seq_t a2 = (seq_t)10;     /* a(n-1)   */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = 14 * a2;              /* 14*a(n-1)           */
        an = an - 63 * a1;         /* -63*a(n-2)           */
        an = an + 90 * a0;         /* +90*a(n-3)           */
        a0 = a1;                   /* shift window: a0 ← a1 */
        a1 = a2;                   /* a1 ← a2               */
        a2 = an;                   /* a2 ← an (current)     */
    }
    return a2;
}
