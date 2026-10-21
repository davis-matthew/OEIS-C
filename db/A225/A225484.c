/*
 * A225484  Expansion of 1/(1 - x^3 - x^4 - x^5 - x^6 + x^9).
 * https://oeis.org/A225484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-4) + a(n-5) + a(n-6) - a(n-9), a(0)=1,
 *            a(1)=0, a(2)=0
 * Range:     u8 n <= 22, u16 n <= 40, u32 n <= 76, u64 n <= 148,
 *            u128 n <= 292, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;           /* allocated capacity */
    static idx_t computed_up_to = 2; /* highest index known to be correct */
    static int initialized = 0;      /* flag for first‑time init of a(0..2) */

    /* Ensure the cache is large enough for index n */
    if (n >= cap) {
        size_t new_cap = (cap == 0) ? 1 : cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* out of memory */
        cache = tmp;

        size_t old_cap = cap;
        cap = new_cap;
        for (size_t i = old_cap; i < cap; ++i) {
#ifdef __cplusplus
            new (&cache[i]) seq_t();      /* default construct (C++ only) */
#else
            cache[i] = (seq_t)0;          /* zero initialisation (C only) */
#endif
        }
    }

    /* Initialise first three values once */
    if (!initialized) {
        cache[0] = (seq_t)1;
        if (cap > 1) cache[1] = (seq_t)0;
        if (cap > 2) cache[2] = (seq_t)0;
        initialized = 1;
    }

    /* Compute any missing terms up to n */
    if (n > computed_up_to) {
        for (idx_t k = computed_up_to + 1; k <= n; ++k) {
            seq_t val = (seq_t)0;
            if (k >= 3) val += cache[k - 3];
            if (k >= 4) val += cache[k - 4];
            if (k >= 5) val += cache[k - 5];
            if (k >= 6) val += cache[k - 6];
            if (k >= 9) val -= cache[k - 9];
            cache[k] = val;
        }
        computed_up_to = n;
    }

    return cache[n];
}
