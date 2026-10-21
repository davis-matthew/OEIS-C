/*
 * A238236  Expansion of (1-x-x^2)/((x-1)*(x^3+3*x^2+2*x-1)).
 * https://oeis.org/A238236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n‑1) + a(n‑2) – 2*a(n‑3) – a(n‑4) Initial values:
 *            a(0)=1, a(1)=2, a(2)=6, a(3)=18
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* Static storage for memoisation */
    static seq_t *tbl = NULL;      /* array of computed values */
    static size_t capacity = 0;    /* number of slots allocated */
    static idx_t max_computed = (idx_t)(-1); /* last computed index,
                                               UINT64_MAX signals “none yet” */

    /* Trivial case: n < 4 – return a constant */
    if (n < 4) {
        static const seq_t init[4] = { (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)18 };
        return init[n];
    }

    /* If we already have the value, return it */
    if (max_computed != (idx_t)(-1) && n <= max_computed) {
        return tbl[n];
    }

    /* Ensure the table can hold indices up to n */
    if (capacity < (size_t)(n + 1)) {
        size_t new_cap = capacity == 0 ? 8 : capacity * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(tbl, new_cap * sizeof(seq_t));
        if (!tmp) { abort(); }
        tbl = tmp;
        capacity = new_cap;
    }

    /* Initialise the first four entries on first use */
    if (max_computed == (idx_t)(-1)) {
        tbl[0] = (seq_t)1;
        tbl[1] = (seq_t)2;
        tbl[2] = (seq_t)6;
        tbl[3] = (seq_t)18;
        max_computed = 3;
    }

    /* Compute values up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        tbl[k] = (seq_t)(
            3 * tbl[k - 1]
          + tbl[k - 2]
          - 2 * tbl[k - 3]
          - tbl[k - 4]
        );
    }
    max_computed = n;
    return tbl[n];
}
