/*
 * A168368  Number of stable connected piles of n bricks.
 * https://oeis.org/A168368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A168368 – Number of stable connected piles of n bricks

   Recurrence (valid for n ≥ 4):
       a(0) = 0
       a(1) = 1
       a(2) = 1
       a(3) = 2
       a(n) = 2·a(n‑1) – a(n‑2) + a(n‑3)
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)1;
    if (n == (idx_t)3) return (seq_t)2;

    seq_t a0 = (seq_t)0;         /* a(n‑3)      */
    seq_t a1 = (seq_t)1;         /* a(n‑2)      */
    seq_t a2 = (seq_t)1;         /* a(n‑1)      */
    seq_t a3 = (seq_t)2;         /* a(n)         */
    seq_t an = a3;

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        an = (seq_t)2 * a3;
        an = an - a2 + a1;       /* 2*a3 – a2 + a1 */

        /* shift the window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
