/*
 * A054516  Equivalent of the Kurepa hypothesis for left factorial.
 * https://oeis.org/A054516
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 11, i32 n <= 15, i64 n <= 23, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(3)=0,
   a(n)=-(n-3)*a(n-1)+(n-3)*(n-2)   for n≥4   (offset 3) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 3)                    /* values below the offset are irrelevant */
        return 0;

    /* ----------- static cache ------------------------------------ */
    static seq_t *cache = NULL;   /* array indexed by n, stores computed terms */
    static idx_t cache_size  = 0; /* allocated size of cache */
    static idx_t max_computed = 2;/* highest n that has been computed so far */

    /* ----- allocate or grow the cache as needed ----------------- */
    if (cache == NULL) {                 /* first call */
        cache_size = n + 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0;            /* out‑of‑memory: give zero (should not happen) */
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = 0;
        cache[3] = 0;                    /* initialise the first defined term */
        max_computed = 3;
    } else if (n + 1 > cache_size) {    /* need more space */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return 0;             /* out‑of‑memory: give zero */
        /* initialise the newly added entries */
        for (idx_t i = cache_size; i < new_size; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* ----- compute missing terms up to n ------------------------ */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t term  = -(i_seq - 3) * cache[i - 1] + (i_seq - 3) * (i_seq - 2);
        cache[i] = term;
    }
    max_computed = n;

    return cache[n];
}
