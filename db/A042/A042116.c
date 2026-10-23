/*
 * A042116  Numerators of continued fraction convergents to sqrt(583).
 * https://oeis.org/A042116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the n‑th convergent of √583
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction of √583 is [24; 6,1,7,5,4,5,7,1,6,48] (period 10). */
/* Recurrence: p_-2 = 0, p_-1 = 1, p_k = a_k·p_(k‑1) + p_(k‑2). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Partial quotients after the initial 24. */
    static const unsigned int period[] = {6, 1, 7, 5, 4, 5, 7, 1, 6, 48};
    const unsigned int period_len = 10;

    seq_t p_prev2 = 0;   /* p_{k-2}  */
    seq_t p_prev1 = 1;   /* p_{k-1}  */
    seq_t p;            /* p_k      */

    for (idx_t k = 0; k <= n; ++k) {
        unsigned int ak = (k == 0) ? 24
                                   : period[(k - 1) % period_len];
        p = (seq_t)ak * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p_prev1;
}
