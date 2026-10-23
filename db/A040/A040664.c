/*
 * A040664  Continued fraction for sqrt(691).
 * https://oeis.org/A040664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Continues fraction terms of sqrt(691) via the standard algorithm
 *  for square‑root continued fractions.  a(0)=floor(sqrt(691)),
 *  a(n)=period[(n-1) mod period_len] for n>=1.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* compute integer floor sqrt of a seq_t value */
static seq_t seq_floor_sqrt(seq_t x)
{
    seq_t r = 0;
    while ((r + 1) * (r + 1) <= x) {
        r += 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* static data for the periodic part */
    static seq_t *period = NULL;
    static size_t period_len = 0;
    static bool initialized = false;
    static seq_t a0;           /* floor(sqrt(691)) */
    static const seq_t D = (seq_t)691;  /* D underflows only if seq_t < 691 */

    if (!initialized) {
        /* initialise a0 */
        a0 = seq_floor_sqrt(D);

        /* build the periodic part */
        /* provisional capacity */
        size_t cap = 16;
        period = (seq_t *)malloc(cap * sizeof(*period));
        if (!period) {
            /* allocation failed – return 0 as sentinel */
            return (seq_t)0;
        }

        seq_t m = 0;
        seq_t d = 1;
        seq_t a_term = a0;

        while (1) {
            /* recurrence for continued fraction of sqrt(D) */
            m = d * a_term - m;
            d = (D - m * m) / d;
            a_term = (a0 + m) / d;

            /* grow array if necessary */
            if (period_len == cap) {
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(period, cap * sizeof(*period));
                if (!tmp) {
                    free(period);
                    return (seq_t)0;
                }
                period = tmp;
            }
            period[period_len++] = a_term;

            /* period ends when a_term == 2*a0 */
            if (a_term == 2 * a0) {
                break;
            }
        }
        initialized = true;
    }

    if (n == 0) {
        return a0;
    }

    /* n >= 1 : index into the periodic part */
    size_t r = (size_t)((n - 1) % (idx_t)period_len);
    return period[r];
}
