/*
 * A042635  Denominators of continued fraction convergents to sqrt(847).
 * https://oeis.org/A042635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042635: Denominators of continued fraction convergents to sqrt(847). */
#include <stdint.h>
#include <stdbool.h>

/* a(0)=1, a(1)=9, a(2)=10, ... */
seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction of sqrt(847).  */
    static const seq_t period[10] = {9,1,2,6,8,6,2,1,9,58};

    if (n == 0) return (seq_t)1;
    if (n == 1) return period[0];

    seq_t q_prev2 = (seq_t)1;      /* q_0   */
    seq_t q_prev1 = period[0];    /* q_1   */
    seq_t q = period[0];

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t a_k = period[(i - 1) % 10];
        q = a_k * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }

    return q;
}
