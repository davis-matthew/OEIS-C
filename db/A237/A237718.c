/*
 * A237718  9-distance Pell numbers.
 * https://oeis.org/A237718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (0 <= n < 9) Recurrence: a(n) = 2 * a(n-9) + a(n-2) for
 *            n >= 9 (OEIS A237718 – 9‑distance Pell numbers)
 * Range:     u8 n <= 32, u16 n <= 59, u32 n <= 115, u64 n <= 225,
 *            u128 n <= 447, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache  = NULL;   /* pointer to array of seq_t */
    static size_t cap    = 0;      /* allocated capacity (elements) */
    static idx_t  last   = (idx_t)-1; /* largest index that has been computed */

    /* initialise the cache on first call */
    if (last == (idx_t)-1) {
        /* start with capacity for 9 elements (indices 0..8) */
        cap = 9;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();  /* fatal error */

        for (idx_t i = 0; i < 9; ++i)
            cache[i] = (seq_t)1;          /* a(0)…a(8) = 1 */

        last = 8;                          /* highest computed index */
    }

    /* grow the cache if needed  */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* compute any missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i < 9)
                cache[i] = (seq_t)1;
            else
                cache[i] = cache[i-9] * 2 + cache[i-2];
        }
        last = n;
    }

    return cache[n];
}
