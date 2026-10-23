/*
 * A041533  Denominators of continued fraction convergents to sqrt(283).
 * https://oeis.org/A041533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 22, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1.  
   For n>=2, a(n) = a(n-1) * p_{n-1} + a(n-2),
   where p_k are the partial quotients of sqrt(283):
   period = {1,4,1,1,1,3,10,1,15,1,10,3,1,1,1,4,1,32}.  */

#include <stdint.h>

static const seq_t period[18] = { (seq_t)1, (seq_t)4, (seq_t)1, (seq_t)1, (seq_t)1,
                                 (seq_t)3, (seq_t)10, (seq_t)1, (seq_t)15,
                                 (seq_t)1, (seq_t)10, (seq_t)3, (seq_t)1,
                                 (seq_t)1, (seq_t)1, (seq_t)4, (seq_t)1,
                                 (seq_t)32 };

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t q_prev2 = (seq_t)1;  /* a(0) */
    seq_t q_prev1 = (seq_t)1;  /* a(1) */
    seq_t q;

    for (idx_t i = 2; i <= n; ++i) {
        /* p_{i-1} = period[(i-1)%18] */
        seq_t p = period[(i - 1) % 18];
        q = p * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }
    return q_prev1;
}
