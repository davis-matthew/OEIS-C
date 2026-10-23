/*
 * A162667  a(n) = 20*a(n-1) - 97*a(n-2) for n > 1; a(0) = 1, a(1) = 10.
 * https://oeis.org/A162667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n‑1) - 97*a(n‑2), a(0) = 1, a(1) = 10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms */
    static size_t cap   = 0;      /* allocated capacity in elements */
    static size_t len   = 0;      /* number of valid terms in cache */

    /* --- initialise cache with the first two terms --- */
    if (cache == NULL) {
        /* start with room for at least two terms */
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort(); /* allocation failure */

        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)10;  /* a(1) */
        len = 2;               /* we now have terms for 0 and 1 */
    }

    /* --- grow cache if the requested index is out of bounds --- */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap;
        while (newcap < (size_t)n + 1)
            newcap <<= 1;     /* use power‑of‑two growth */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) abort(); /* realloc failure */
        cache = tmp;
        cap   = newcap;
    }

    /* --- compute any missing terms up to n --- */
    if ((size_t)n + 1 > len) {
        for (idx_t i = (idx_t)len; i <= n; ++i) {
            /* the recurrence uses two previous terms */
            seq_t term = (seq_t)20 * cache[i - 1] - (seq_t)97 * cache[i - 2];
            cache[i] = term;
        }
        len = (size_t)n + 1;
    }

    return cache[n];
}
