/*
 * A041208  Numerators of continued fraction convergents to sqrt(115).
 * https://oeis.org/A041208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 28, u64 n <= 57, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A041208:
   a(0) = 10,
   a(1) = 11,
   a(n) = q(n) * a(n-1) + a(n-2)  for n >= 2,
   where q(n) are the partial quotients of the continued fraction of sqrt(115)
   (periodic: [1,2,1,1,1,1,1,1,2,1,20,1,…]). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* special cases */
    if (n == 0) return (seq_t)10;
    const seq_t a0 = (seq_t)10;     /* floor(sqrt(115)) */
    const seq_t D  = (seq_t)115;    /* D of sqrt(D) */

    /* state for continued fraction generation */
    seq_t m      = (seq_t)0;
    seq_t d      = (seq_t)1;
    seq_t a_prev = a0;   /* a_{k-1} for the iteration (starts with a0) */

    /* state for convergent numerators */
    seq_t prev2 = (seq_t)1; /* p_{-1} = 1 */
    seq_t prev1 = a0;       /* p_0 = a0 */
    seq_t curr  = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        /* next partial quotient a_k */
        m      = d * a_prev - m;
        d      = (D - m * m) / d;
        seq_t a_curr = (a0 + m) / d;

        /* convergent numerator p_k */
        curr = a_curr * prev1 + prev2;

        if (k == n) return curr;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = curr;
        a_prev = a_curr;
    }

    /* should never reach here for n >= 0 */
    return curr;
}
