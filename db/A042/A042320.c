/*
 * A042320  Numerators of continued fraction convergents to sqrt(687).
 * https://oeis.org/A042320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n‑th convergent to sqrt(687), computed
 *            by the recurrence p_{-1} = 1, p_0 = 26, p_k = c_k * p_{k-1} +
 *            p_{k-2} for k >= 1, where c_k cycles through {4, 1, 2, 1, 16, 1,
 *            2, 1, 4, 52}
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const int coef[] = {4, 1, 2, 1, 16, 1, 2, 1, 4, 52};
    const size_t period = sizeof coef / sizeof coef[0];

    if (n == 0) return (seq_t)26;   /* a(0) */

    /* Recurrence initialization */
    seq_t p_prev = 1;   /* p_{-1} */
    seq_t p_curr = 26;  /* p_0 */

    for (idx_t k = 1; k <= n; ++k)
    {
        int c = coef[(k - 1) % period];
        seq_t next = (seq_t)c * p_curr + p_prev; /* p_k = c * p_{k-1} + p_{k-2} */
        p_prev = p_curr;
        p_curr = next;
    }

    return p_curr;
}
