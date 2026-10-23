/*
 * A041690  Numerators of continued fraction convergents to sqrt(365).
 * https://oeis.org/A041690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the numerators of the continued fraction convergents to sqrt(365):
   a(0) = 19, a(-1) = 1
   a(n) = p_n where p_n = a_n * p_{n-1} + p_{n-2}
   Partial quotients a_n repeat with period [9, 1, 1, 9, 38]. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)19;                 /* a(0) = 19 */

    static const idx_t period[5] = {9, 1, 1, 9, 38};
    seq_t p_prev = 1;                   /* p_{-1} */
    seq_t p_curr = 19;                  /* p_0 */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t ai = (seq_t)period[(i - 1) % 5]; /* a_i */
        seq_t p_next = ai * p_curr + p_prev;  /* p_i = a_i*p_{i-1} + p_{i-2} */
        p_prev = p_curr;
        p_curr = p_next;
    }

    return p_curr;                      /* p_n */
}
