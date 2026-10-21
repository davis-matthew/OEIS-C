/*
 * A237255  Values of x in the solutions to x^2 - 5xy + y^2 + 17 = 0, where 0
 *          < x < y.
 * https://oeis.org/A237255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 57, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-2) - a(n-4)  (n ≥ 5)
   Base terms: a(1) = 2, a(2) = 3, a(3) = 7, a(4) = 13 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n = 0 */

    /* Static cache: cache[i] holds a(i+1) for i = 0 … used-1 */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated buffer size    */
    static size_t used = 0;         /* number of terms computed */

    /* Initial allocation and base terms */
    if (cache == NULL) {
        size_t init = 5;                         /* at least 5 entries */
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache)
            exit(1);
        cache[0] = (seq_t)2;   /* a(1) */
        cache[1] = (seq_t)3;   /* a(2) */
        cache[2] = (seq_t)7;   /* a(3) */
        cache[3] = (seq_t)13;  /* a(4) */
        cap  = init;
        used = 4;              /* four terms computed */
    }

    /* Need at least n terms (zero‑based index n‑1). */
    size_t need = (size_t)n;
    if (used < need) {
        /* Grow allocation if necessary. */
        if (cap < need) {
            size_t new_cap = cap * 2;
            if (new_cap < need)
                new_cap = need;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!cache)
                exit(1);
            cap = new_cap;
        }
        /* Compute missing terms using the 4‑term recurrence. */
        for (size_t i = used; i < need; ++i) {
            /* i ≥ 4 (since used ≥ 4) */
            seq_t term = (seq_t)5 * cache[i - 2] - cache[i - 4];
            cache[i] = term;
        }
        used = need;
    }

    return cache[n - 1];
}
