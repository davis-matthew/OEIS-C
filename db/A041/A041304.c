/*
 * A041304  Numerators of continued fraction convergents to sqrt(165).
 * https://oeis.org/A041304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041304: Numerators of continued fraction convergents to sqrt(165)
 * Recurrence: p_-2 = 0, p_-1 = 1
 *             p_n  = a_n * p_(n-1) + p_(n-2)
 * where a_0 = 12 and for n ≥ 1: a_n follows the repeating pattern
 * (1,5,2,5,1,24).  This pattern corresponds to the periodic continued
 * fraction [12; (1,5,2,5,1,24) repeating] for sqrt(165).
 * The function computes p_n (the nth numerator) for any non‑negative n.
 */
#include <stddef.h>

static const int period[6] = {1, 5, 2, 5, 1, 24};

seq_t a(idx_t n)
{
    /* Initial values for the recurrence: p_-2 = 0, p_-1 = 1 */
    seq_t p_prevprev = 0; /* p_{-2} */
    seq_t p_prev    = 1;  /* p_{-1} */
    seq_t p_curr;         /* placeholder for p_n */

    /* Compute p_n iteratively */
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t a_coef;

        if (i == 0)
            a_coef = (seq_t)12;
        else
        {
            size_t m = (size_t)((i - 1) % 6); /* cycle through the period */
            a_coef = (seq_t)period[m];
        }

        p_curr     = a_coef * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev     = p_curr;
    }

    return p_prev; /* p_n */
}
