/*
 * A041734  Numerators of continued fraction convergents to sqrt(387).
 * https://oeis.org/A041734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   Numerators of continued fraction convergents to sqrt(387).
 *            Recurrence: p[-2] = 0, p[-1] = 1, p[n] = a[n] * p[n-1] + p[n-2]
 *            (n ≥ 0) where a[0] = 19 and the period for a[1]..a[6] is 1, 2,
 *            19, 2, 1, 38 (then repeats)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements a(idx_t n) returning a(n). */
#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Partial quotients of sqrt(387) up to one full period. */
    static const seq_t a_vals[7] = {
        (seq_t)19, /* a[0] */
        (seq_t)1,  /* a[1] */
        (seq_t)2,  /* a[2] */
        (seq_t)19, /* a[3] */
        (seq_t)2,  /* a[4] */
        (seq_t)1,  /* a[5] */
        (seq_t)38  /* a[6] */
    };

    seq_t p_prevprev = 0;  /* p[-2] */
    seq_t p_prev     = 1;  /* p[-1] */
    seq_t p_curr     = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t ai;
        if (i == 0)
            ai = a_vals[0];
        else
        {
            /* After the initial term, the partial quotients repeat every 6. */
            size_t k = ((size_t)(i - 1) % 6) + 1;  /* k in 1..6 */
            ai = a_vals[k];
        }
        p_curr = ai * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev = p_curr;
    }

    return p_curr;
}
