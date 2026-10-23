/*
 * A159472  Numerator of Hermite(n, 1/12).
 * https://oeis.org/A159472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = a(n-1) - 72*(n-1)*a(n-2), with a(0)=1, a(1)=1
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 14, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;  /* dynamic array of computed values      */
static size_t cap   = 0;     /* allocated capacity (in elements)      */
static idx_t last   = 0;     /* highest index that has been computed  */

/* grow the cache to hold at least `need` elements */
static void grow_cache(idx_t need)
{
    if (cap >= (size_t)need) return;
    size_t new_cap = cap ? cap : 1;
    while (new_cap < (size_t)need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failed – terminate */
    cache = tmp;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache with the first two terms if empty */
    if (!cache) {
        cache = (seq_t*)calloc(2, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;
        cache[1] = 1;
        cap   = 2;
        last  = 1;                /* indices 0 and 1 are ready */
    }

    /* if the requested index is beyond our current cache, grow it */
    grow_cache(n + 1);

    /* compute any missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i == 0 || i == 1) {      /* already set, but keep for safety */
                continue;
            }
            seq_t term = cache[i - 1];
            seq_t mult = cache[i - 2];
            mult *= (seq_t)(i - 1);      /* (i-1) */
            mult *= 72;                 /* 72*(i-1) */
            term -= mult;
            cache[i] = term;
        }
        last = n;
    }

    return cache[n];
}
