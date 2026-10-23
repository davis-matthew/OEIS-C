/*
 * A041049  Denominators of continued fraction convergents to sqrt(30).
 * https://oeis.org/A041049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of convergents of sqrt(30).  
   Recurrence using the continued‑fraction expansion  
   sqrt(30) = [5;overline{2,10}].  
   Let a_k be the k‑th partial quotient:

        a_0 = 5,
        a_{2m+1} = 2,  a_{2m+2} = 10   (m ≥ 0).

   Initialize:
        p_{-2} = 0, p_{-1} = 1,
        q_{-2} = 1, q_{-1} = 0.

   For n ≥ 0:
        p_n = a_n * p_{n-1} + p_{n-2}
        q_n = a_n * q_{n-1} + q_{n-2}

   The function returns q_n, the denominator of the n‑th convergent. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t p_prev2 = 0, p_prev1 = 1, p;
    seq_t q_prev2 = 1, q_prev1 = 0, q;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak = (k == 0) ? (seq_t)5
                            : ((k & 1) ? (seq_t)2 : (seq_t)10);
        p = ak * p_prev1 + p_prev2;
        q = ak * q_prev1 + q_prev2;
        p_prev2 = p_prev1;  p_prev1 = p;
        q_prev2 = q_prev1;  q_prev1 = q;
    }
    return q_prev1;
}
