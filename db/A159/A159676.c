/*
 * A159676  Numerator of Hermite(n, 17/20).
 * https://oeis.org/A159676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 17, a(n) = 17*a(n-1) - 200*(n-1)*a(n-2) for n
 *            >= 2 (derived from the recurrence
 *            a(n)-17*a(n-1)+200*(n-1)*a(n-2)=0)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Memoised recurrence with static cache */
seq_t a(idx_t n) {
    /* --- base cases, no caching needed for n < 2 --- */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)17;

    /* --- static cache for arbitrary-bit sequences --- */
    static seq_t* cache = NULL;       /* array of values a(0)...a(i) */
    static size_t cache_cap = 0;      /* allocated capacity */
    static idx_t cache_upto = (idx_t)-1; /* last index computed */

    /* --- initialise cache on first use --- */
    if (cache == NULL) {
        /* allocate enough space for at least n+1 entries */
        size_t initial_cap = n + 1;
        if (initial_cap < 2) initial_cap = 2;
        cache = (seq_t*)malloc(initial_cap * sizeof(seq_t));
        if (cache == NULL) {exit(EXIT_FAILURE);}  /* silence any compiler warning */
        cache_cap = initial_cap;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)17;
        cache_upto = 1;
    }

    /* --- grow cache if needed --- */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
        seq_t* tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {exit(EXIT_FAILURE);}
        cache = tmp;
        cache_cap = new_cap;
    }

    /* --- compute missing entries --- */
    for (idx_t i = cache_upto + 1; i <= n; ++i) {
        /* compute a(i) = 17*a(i-1) - 200*(i-1)*a(i-2) */
        seq_t term1 = (seq_t)17 * cache[i-1];
        seq_t term2 = (seq_t)200 * (i-1) * cache[i-2];
        cache[i] = term1 - term2;
    }
    cache_upto = n;

    /* --- return result --- */
    return cache[n];
}
