/*
 * A042731  Denominators of continued fraction convergents to sqrt(895).
 * https://oeis.org/A042731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042731: Denominators of continued fraction convergents to sqrt(895). */
/* Partial quotients: a0 = 29, then repeat 1,10,1,58. */
/* Denominators q_n satisfy q_{-2}=1, q_{-1}=0, q_n = a_n*q_{n-1}+q_{n-2}. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed denominators */
    static seq_t *den = NULL;
    static idx_t sz = 0;
    static seq_t qprevprev = (seq_t)1, qprev = (seq_t)0;

    /* pattern of partial quotients for n>=1 */
    static const seq_t pattern[4] = {(seq_t)1, (seq_t)10, (seq_t)1, (seq_t)58};

    if (sz <= n) {                     /* need to extend the cache */
        size_t new_size = (size_t)(n + 1);

        if (!den)
            den = (seq_t*)malloc(new_size * sizeof(seq_t));
        else
            den = (seq_t*)realloc(den, new_size * sizeof(seq_t));

        if (!den)
            abort();                  /* allocation failed */

        if (sz == 0) {                /* initialise recurrence state */
            qprevprev = (seq_t)1;
            qprev     = (seq_t)0;
        }

        for (idx_t i = sz; i <= n; ++i) {
            seq_t a_i;
            if (i == 0)
                a_i = (seq_t)29;
            else
                a_i = pattern[(size_t)((i - 1) % 4)];

            seq_t q_i = a_i * qprev + qprevprev;
            den[i] = q_i;

            /* shift recurrence state */
            qprevprev = qprev;
            qprev     = q_i;
        }

        sz = n + 1;
    }

    return den[n];
}
