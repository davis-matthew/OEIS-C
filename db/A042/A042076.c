/*
 * A042076  Numerators of continued fraction convergents to sqrt(562).
 * https://oeis.org/A042076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(562)
   a(0) = 23
   a(1) = 24
   For n >= 1:
       let a_n be the nth partial quotient of sqrt(562)
       with a_0 = 23 and the repeating block
             1, 2, 2, 2, 4, 1, 5, 1, 22, 1, 5, 1, 4, 2, 2, 2, 1, 46
       (period 18, a_n = block[(n-1) mod 18] for n >= 1)
   The convergent numerators satisfy:
       p[-1] = 1,  p[0] = 23
       p[n]   = a_n * p[n-1] + p[n-2]   (n >= 1)
   The function returns p[n] for any n ≥ 0, computing the value iteratively.
   No dynamic memory allocation is performed, so it works for arbitrary-precision seq_t
   as well as for any unsigned integer size. */

#include <stddef.h>

static const seq_t _period[18] = {
    1, 2, 2, 2, 4, 1, 5, 1, 22, 1, 5, 1, 4, 2, 2, 2, 1, 46
};

seq_t a(idx_t n)
{
    /* Handle the trivial cases directly */
    if (n == 0) return (seq_t)23;
    /* p[-1] = 1, p[0] = 23  */
    seq_t p_prev_prev = (seq_t)1;   /* p[-1] */
    seq_t p_prev      = (seq_t)23;  /* p[0] */
    seq_t p_curr;

    for (idx_t i = 1; i <= n; ++i) {
        /* a_i for i >= 1 */
        seq_t coeff = _period[(i - 1) % 18];
        /* recurrence: p_i = coeff * p_{i-1} + p_{i-2} */
        p_curr = coeff * p_prev + p_prev_prev;
        /* shift for next iteration */
        p_prev_prev = p_prev;
        p_prev      = p_curr;
    }
    return p_curr;
}
