/*
 * A042661  Denominators of continued fraction convergents to sqrt(860).
 * https://oeis.org/A042661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence based on continued fraction of sqrt(860):
   Let a(0)=1, a(-1)=0.
   For n ≥ 0 let
        c_n = 29                if n==0
        c_n = period[(n-1)%4]   otherwise,
   where period = [3, 14, 3, 58].
   Then
        a(n) = c_n * a(n-1) + a(n-2).
   This yields the denominators of the convergents to sqrt(860). */

#include <stdint.h>

static const seq_t period[4] = {(seq_t)3, (seq_t)14, (seq_t)3, (seq_t)58};

seq_t a(idx_t n)
{
    seq_t q_prev2 = (seq_t)1; /* a(-2) */
    seq_t q_prev1 = (seq_t)0; /* a(-1) */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t coeff = (i == 0) ? (seq_t)29 : period[(i - 1) % 4];
        seq_t q = coeff * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }
    return q_prev1;
}
