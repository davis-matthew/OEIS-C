/*
 * A178510  a(n+1) = a(n)*100 + 21^n, with a(1)=1.
 * https://oeis.org/A178510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n+1) = a(n)*100 + 21^n Linear recurrence used: a(n)
 *            = 121*a(n-1) – 2100*a(n-2) (valid for n ≥ 3)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static memoisation */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* allocated capacity (index size)   */
    static idx_t last_cached = 0;  /* largest index computed so far     */

    /* --- allocation helper ---------------------------------------- */
    /* ensures we have room for indices up to 'needed-1' (0‑based)      */
    if (n + 1 > cap) {
        /* grow to at least n+1, doubling each time for amortised O(1) */
        size_t new_cap = cap ? cap << 1 : 1;
        while (new_cap <= n) new_cap <<= 1;

        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_cap];
#else
        new_cache = malloc(new_cap * sizeof(seq_t));
#endif
        if (!new_cache)
            exit(EXIT_FAILURE);          /* memory exhausted */

        /* copy existing data if any */
        if (cache) {
            for (idx_t i = 0; i < cap; ++i)
                new_cache[i] = cache[i];
        }

        /* free the old block */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cap = new_cap;
    }
    /* --------------------------------------------------------------- */

    /* initialise base cases on the first call */
    if (last_cached == 0) {
        cache[1] = (seq_t)1;                /* a(1) */
        last_cached = 1;
    }
    if (last_cached < 2 && n >= 2) {
        cache[2] = (seq_t)121;              /* a(2) */
        last_cached = 2;
    }

    /* compute the sequence up to requested n */
    for (idx_t k = last_cached + 1; k <= n; ++k) {
        cache[k] = (seq_t)121 * cache[k - 1]
                 - (seq_t)2100 * cache[k - 2];
    }
    last_cached = n;

    return cache[n];
}
