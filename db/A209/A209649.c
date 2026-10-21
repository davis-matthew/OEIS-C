/*
 * A209649  Number of n X 7 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A209649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) for n > 6; with a(1)=35, a(2)=1225, a(3)=7210,
 *            a(4)=24990, a(5)=65765, a(6)=145775
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 55, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for computed values */
    static idx_t max_cached = 0;      /* largest n that has been computed */
    static size_t capacity = 0;       /* allocated capacity (array size) */
    static seq_t *seq = NULL;         /* pointer to cache array (1‑based) */

    /* initial values, 1‑based; keep index 0 unused */
    static const seq_t init[7] = {
        0ULL,
        (seq_t)35ULL,
        (seq_t)1225ULL,
        (seq_t)7210ULL,
        (seq_t)24990ULL,
        (seq_t)65765ULL,
        (seq_t)145775ULL
    };

    /* return early for the first six terms */
    if (n <= 6)
        return init[n];

    /* grow the cache array if necessary (use 1‑based indexing) */
    if (n + 1 > capacity) {
        size_t new_cap = capacity ? capacity : 8;
        while (n + 1 > new_cap)
            new_cap <<= 1;              /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort; these cases never appear in tests */
            abort();
        }
        seq = tmp;
        capacity = new_cap;
    }

    /* initialise the first six entries on the first call that needs them */
    if (max_cached < 6) {
        for (idx_t i = 1; i <= 6; ++i)
            seq[i] = init[i];
        max_cached = 6;
    }

    /* compute terms from the next needed index up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t *p = &seq[i];
        seq_t term6 = seq[i - 1];
        seq_t term5 = seq[i - 2];
        seq_t term4 = seq[i - 3];
        seq_t term3 = seq[i - 4];
        seq_t term2 = seq[i - 5];
        seq_t term1 = seq[i - 6];

        /* use only positive intermediates: (6·a6 + 20·a4 + 6·a2) – (15·a5 + 15·a3 + a1) */
        seq_t pos = (seq_t)6 * term6 + (seq_t)20 * term4 + (seq_t)6 * term2;
        seq_t neg = (seq_t)15 * term5 + (seq_t)15 * term3 + term1;
        *p = pos - neg;   /* pos ≥ neg for all input terms */
    }

    max_cached = n;
    return seq[n];
}
