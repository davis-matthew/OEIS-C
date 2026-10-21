/*
 * A059822  Expansion of series related to Liouville's Last Theorem: g.f.
 *          sum_{t>0} (-1)^(t+1) *x^(t*(t+1)/2) / ( (1-x^t)^5
 *          *product_{i=1..t} (1-x^i) ).
 * https://oeis.org/A059822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in sum_{t>0} (-1)^(t+1) *
 *            x^(t*(t+1)/2) / ((1-x^t)^5 * product_{i=1..t} (1-x^i))
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the sequence by expanding the
   generating function directly.  For each t we maintain the
   partition numbers with parts up to t-1 in an array `part`.
   The factor (1-x^t)^-6 is applied by 6 successive convolutions
   with (1-x^t)^-1, which amounts to a simple repeated addition
   over a stride of t.  The result is then shifted by T_t and
   added to the final coefficients with the appropriate sign. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed values */
    static seq_t *val = NULL;
    static idx_t val_size = 0;

    if (n < val_size)          /* already known */
        return val[n];

    /* recompute the coefficients up to n from scratch */
    idx_t N = n;
    idx_t size = N + 1;

    /* allocate temporary arrays */
    seq_t *a_seq = (seq_t*)malloc(size * sizeof(seq_t));
    seq_t *part  = (seq_t*)malloc(size * sizeof(seq_t));
    seq_t *conv  = (seq_t*)malloc(size * sizeof(seq_t));

    /* initialise */
    for (idx_t i = 0; i < size; ++i) {
        a_seq[i] = 0;
        part[i]  = 0;
        conv[i]  = 0;
    }
    part[0] = 1;                 /* product over empty set = 1 */

    /* determine maximal t such that T_t <= N */
    idx_t tmax = 0;
    while ((tmax + 1) * (tmax + 2) / 2 <= N)
        ++tmax;

    /* main loop over t */
    for (idx_t t = 1; t <= tmax; ++t) {
        idx_t shift = t * (t + 1) / 2;
        int  sign  = (t & 1) ? 1 : -1;   /* (-1)^(t+1) */

        /* copy current partition array to conv */
        for (idx_t i = 0; i < size; ++i)
            conv[i] = part[i];

        /* apply (1 - x^t)^-6 by 6 successive convolutions */
        for (int pass = 0; pass < 6; ++pass) {
            for (idx_t idx = t; idx <= N; ++idx)
                conv[idx] = conv[idx] + conv[idx - t];
        }

        /* add the contribution of this t to the final sequence */
        if (sign == 1) {
            for (idx_t i = shift; i <= N; ++i)
                a_seq[i] = a_seq[i] + conv[i - shift];
        } else {
            for (idx_t i = shift; i <= N; ++i)
                a_seq[i] = a_seq[i] - conv[i - shift];
        }

        /* update 'part' array to include the part t for the next step */
        for (idx_t idx = t; idx <= N; ++idx)
            part[idx] = part[idx] + part[idx - t];
    }

    /* free old cache */
    if (val != NULL)
        free(val);

    /* use the newly computed array as cache */
    val = a_seq;
    val_size = size;

    /* part and conv can be freed */
    free(part);
    free(conv);

    return val[n];
}
