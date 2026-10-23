/*
 * A042806  Numerators of continued fraction convergents to sqrt(934).
 * https://oeis.org/A042806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(934).
   Recurrence: 
     p_-1 = 1
     p_0  = 30
     p_n  = a_n * p_{n-1} + p_{n-2}      for n ≥ 1
   with periodic coefficients a_n (period 14):
     1, 1, 3, 1, 1, 3, 30, 3, 1, 1, 3, 1, 1, 60
   a(0) = 30 is the first term of the sequence. */
#include <stdint.h>

static const int coef[14] = {
    1, 1, 3, 1, 1, 3,
    30, 3, 1, 1, 3, 1,
    1, 60
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 30;

    seq_t p_prev = 1;   /* p_{-1} */
    seq_t p_curr = 30;  /* p_0  */

    for (idx_t i = 1; i <= n; ++i) {
        int c = coef[(i - 1) % 14];
        seq_t next = (seq_t)c * p_curr + p_prev;
        p_prev = p_curr;
        p_curr = next;
    }
    return p_curr;
}
