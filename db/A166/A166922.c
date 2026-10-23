/*
 * A166922  E.g.f. exp(-x)*exp(exp(2*x)/2-1/2)/2 + 1/2.
 * https://oeis.org/A166922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let g(0)=1 and for n≥1 define g(n) = ∑_{k=2}^{n} 2^{k-1} ·
 *            C(n-1,k-1) · g(n-k) where C(·,·) is the binomial coefficient.
 *            Then the required sequence satisfies a(0) = 1, a(n) = g(n) / 2
 *            for n ≥ 1. This recurrence follows from the e.g.f
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return a(n) for the OEIS sequence A166922. */
seq_t a(idx_t n)
{
    /* static cache of g(n) values */
    static seq_t *g = NULL;
    static idx_t size = 0;      /* number of g values stored (size = max computed + 1) */

    /* enlarge cache if necessary */
    if (n + 1 > size) {
        idx_t old_size = size;
        size = n + 1;

        /* allocate new array */
        seq_t *new_g = (seq_t *)malloc(size * sizeof(seq_t));
        if (!new_g) { /* allocation failure – abort execution */
            abort();
        }

        /* copy old values */
        for (idx_t i = 0; i < old_size; ++i)
            new_g[i] = g[i];

        /* initialise new slots to zero (not really needed but clean) */
        for (idx_t i = old_size; i < size; ++i)
            new_g[i] = 0;

        /* free old array */
        if (g)
            free(g);

        g = new_g;

        /* compute new g values from old_size up to n */
        for (idx_t m = old_size; m <= n; ++m) {
            if (m == 0) {
                g[0] = 1;
                continue;
            }
            seq_t sum = 0;
            seq_t C   = 1;   /* C(m-1,0) */
            seq_t h   = 1;   /* 2^{0} for j=0 (k=j+1) */

            /* loop over j = k-1 from 1 to m-1 (i.e. k = 2..m) */
            for (idx_t j = 1; j <= m - 1; ++j) {
                /* update binomial coefficient C(m-1,j) from C(m-1,j-1) */
                seq_t num = (seq_t)(m - 1 - (j - 1));
                C = (C * num) / (seq_t)j;   /* exact division */

                /* double h to get 2^{j} = h_k for k=j+1 */
                h = h * (seq_t)2;

                /* term = h * C * g[m-1-j]  (since m-k = m-1-j) */
                seq_t term = (h * C) * g[m - 1 - j];
                sum += term;
            }
            g[m] = sum;
        }
    }

    /* compute a(n) from stored g(n) */
    seq_t result = g[n];
    if (n > 0)
        result /= 2;          /* division is exact for n≥1 */
    return result;
}
