/*
 * A144368  Shifts 4 places left under Dirichlet convolution.
 * https://oeis.org/A144368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  eigen, nonn
 * Formula:   a(n) = sum_{d | (n-4)} a(d) * a((n-4)/d) for n >= 5, with a(1) =
 *            a(2) = a(3) = a(4) = 1
 * Range:     u8 n <= 27, u16 n <= 59, u32 n <= 123, u64 n <= 251,
 *            u128 n <= 507, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers for memory allocation and size types */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for the first four indices */
    if (n <= 4)
        return (seq_t)1;

    /* Static cache for previously computed values */
    static seq_t *vals = NULL;   /* array index 0 unused; vals[i] = a(i) */
    static size_t cap = 0;       /* allocated capacity (elements) */
    static size_t computed = 0;  /* highest index computed and stored */

    /* Ensure cache is allocated */
    size_t need = (size_t)n + 1;            /* need space for index n */
    if (vals == NULL) {
        cap = (need > 16 ? need : 16);      /* start with at least 16 */
        vals = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!vals)
            return (seq_t)0;                /* allocation failure */
        for (size_t i = 0; i < cap; ++i)
            vals[i] = (seq_t)0;             /* zero‑initialise */

        /* Initialise base terms */
        vals[1] = vals[2] = vals[3] = vals[4] = (seq_t)1;
        computed = 4;
    } else if (n >= (idx_t)cap) {
        /* Grow cache if required */
        size_t newcap = cap;
        while (newcap <= (size_t)n)
            newcap <<= 1;                   /* double size each time */
        seq_t *tmp = (seq_t *)realloc(vals, sizeof(seq_t) * newcap);
        if (!tmp)
            return (seq_t)0;
        for (size_t i = cap; i < newcap; ++i)
            tmp[i] = (seq_t)0;
        vals = tmp;
        cap = newcap;
    }

    /* Compute values up to requested index n if not already done */
    for (size_t k = computed + 1; k <= (size_t)n; ++k) {
        idx_t m = (idx_t)k - 4;          /* m = n - 4 */
        seq_t s = (seq_t)0;

        /* Sum over divisor pairs (ordered) */
        for (idx_t d = 1; d <= m / d; ++d) {
            if (m % d == 0) {
                idx_t q = m / d;         /* quotient */
                if (d == q) {
                    s += vals[d] * vals[q];
                } else {
                    seq_t prod = vals[d] * vals[q];
                    s += prod;          /* (d, q) */
                    s += prod;          /* (q, d) */
                }
            }
        }
        vals[k] = s;
    }
    computed = (size_t)n;

    return vals[n];
}
