/*
 * A163347  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1)+6*a(n-2)+6*a(n-3)+6*a(n-4)-21*a(n-5) (n≥6)
 *            a(0)=1, a(1)=8, a(2)=56, a(3)=392, a(4)=2744, a(5)=19180
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the first five terms explicitly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)8;
    if (n == 2u) return (seq_t)56;
    if (n == 3u) return (seq_t)392;
    if (n == 4u) return (seq_t)2744;
    if (n == 5u) return (seq_t)19180;

    /* initialise sliding window with the first five values */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)8;
    seq_t a2 = (seq_t)56;
    seq_t a3 = (seq_t)392;
    seq_t a4 = (seq_t)2744;

    /* compute a(5) with the constant +1 from the generating function */
    seq_t a5 = 6*a4 + 6*a3 + 6*a2 + 6*a1 - 21*a0 + (seq_t)1;
    if (n == 5u) return a5;

    /* shift the window to start the normal recurrence from n = 6 */
    a0 = a1;
    a1 = a2;
    a2 = a3;
    a3 = a4;
    a4 = a5;

    for (idx_t i = 6; i <= n; ++i) {
        seq_t ai = 6*a4 + 6*a3 + 6*a2 + 6*a1 - 21*a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }

    return a4;
}
