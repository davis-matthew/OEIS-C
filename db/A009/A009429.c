/*
 * A009429  E.g.f. log(1+x)/cos(x).
 * https://oeis.org/A009429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the egf log(1+x)/cos(x)
 *            Recurrence: a(0) = 0 a(n) = (-1)^(n-1)·(n-1)! − Σ_{t even, 0 < t
 *            ≤ n} C(n,t)·(-1)^{t/2}·a(n‑t) where C(n,t) = n!/(t!(n‑t)!). The
 *            implementation memoises previously computed values
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* for realloc */
#include <stdint.h> /* for uint64_t */

seq_t a(idx_t n) {
    /* memoisation state */
    static seq_t *seq = NULL;          /* sequence array */
    static idx_t  cap  = 0;            /* current capacity (number of slots) */
    static seq_t  fact = 0;            /* (m-1)! for the next m to compute */

    /* extend storage to include n */
    if (n >= cap) {
        size_t new_cap = (cap > 1 ? cap << 1 : 1);
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!seq) exit(1);

        if (cap == 0) { /* initialisation */
            seq[0] = (seq_t)0;
            fact   = (seq_t)1; /* 0! = 1 */
            cap    = 1;
        }

        /* compute values from old cap up to new_cap-1 */
        for (idx_t m = cap; m < new_cap; ++m) {
            if (m == 0) {          /* already set */
                seq[0] = (seq_t)0;
                continue;
            }

            /* term (-1)^(m-1)*(m-1)! */
            seq_t bm = fact;
            if (((idx_t)(m - 1) & 1ULL)) bm = -bm;

            /* sum over even t */
            seq_t sum = (seq_t)0;
            seq_t C   = (seq_t)1;   /* C(m,0) */

            for (idx_t t = 1; t <= m; ++t) {
                /* update C to C(m,t) */
                C = C * (seq_t)(m - t + 1);
                C = C / (seq_t)t;

                if ((t & 1ULL) == 0) {          /* t even */
                    idx_t k = m - t;            /* index of a(k) */
                    seq_t term = C * seq[k];

                    if (((t >> 1) & 1ULL)) term = -term; /* (-1)^(t/2) */

                    sum += term;
                }
            }

            seq[m] = bm - sum;

            /* update factorial to m! */
            fact = fact * (seq_t)m;
        }

        cap = new_cap;
    }

    return seq[n];
}
