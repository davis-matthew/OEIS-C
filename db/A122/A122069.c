/*
 * A122069  a(n) = 3*a(n-1) + 9*a(n-2) for n > 1, with a(0)=1, a(1)=3.
 * https://oeis.org/A122069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 9*a(n-2) for n > 1, with a(0) = 1, a(1) = 3.
 *            The sequence satisfies the recurrence a(n) = 3*a(n-1) + 9*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated array size (in elements) */
    static idx_t computed_last = 1; /* highest index already computed */

    /* --------------------------------------------------------------------
       First invocation: allocate cache and set base cases.
       -------------------------------------------------------------------- */
    if (!cache) {
        /* Allocate enough space for at least the first two terms.
           If the initial index is large, pre‑allocate that much to avoid a
           needless reallocation later. */
        cap = (n + 1) > 2 ? (n + 1) : 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)       /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        computed_last = 1;
    }

    /* --------------------------------------------------------------------
       Grow the cache if needed.
       -------------------------------------------------------------------- */
    if ((size_t)n >= cap) {
        size_t new_cap = (n + 1) > cap ? (n + 1) : cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)       /* reallocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        cache = tmp;
        cap = new_cap;
    }

    /* --------------------------------------------------------------------
       Return cached term if already computed.
       -------------------------------------------------------------------- */
    if (n <= (idx_t)computed_last) {
        return cache[(size_t)n];
    }

    /* --------------------------------------------------------------------
       Compute terms up to index n using the recurrence.
       -------------------------------------------------------------------- */
    for (idx_t i = computed_last + 1; i <= n; ++i) {
        seq_t term = 3 * cache[(size_t)(i - 1)] + 9 * cache[(size_t)(i - 2)];
        cache[(size_t)i] = term;
    }
    computed_last = n;

    return cache[(size_t)n];
}
