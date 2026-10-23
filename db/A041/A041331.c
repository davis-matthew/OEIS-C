/*
 * A041331  Denominators of continued fraction convergents to sqrt(179).
 * https://oeis.org/A041331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (OEIS A041331):  
   a(0) = 1;  
   For n ≥ 1 let p = {2,1,1,1,3,5,13,5,3,1,1,1,2,26} (periodic part of the
   continued fraction for √179).  Then  
   a(n) = p[(n‑1) mod 14] * a(n‑1) + a(n‑2).
   The indices start at 0.  This gives the denominators of the
   convergents to √179. */

#include <stddef.h>

static const int period[14] = {2,1,1,1,3,5,13,5,3,1,1,1,2,26};

seq_t a(idx_t n)
{
    /* a(0) is 1 by definition */
    if (n == 0)
        return (seq_t)1;

    seq_t q_prev2 = 0;   /* q_{-1} */
    seq_t q_prev1 = 1;   /* q_{0}  */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* partial quotient a_k (k starts at 1) */
        int a_k = period[(k - 1) % 14];
        /* recurrence: q_k = a_k * q_{k-1} + q_{k-2} */
        seq_t q = (seq_t)a_k * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }

    return q_prev1;
}
