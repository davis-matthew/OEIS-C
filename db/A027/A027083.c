/*
 * A027083  a(n) = A027082(n, n+2).
 * https://oeis.org/A027083
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 72, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A027083
   Definition: a(n) = A027024(n) + 1
   Recurrence (valid for n ≥ 6) : a(n) = 2 * a(n-1) – a(n-4)
   Base values: a(2)=2, a(3)=6, a(4)=14, a(5)=28
   (Terms with n<2 are undefined; the function returns 0.) */
       
#include <stdlib.h>      /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* Off‑by‑offset: for indices below 2, the value is undefined */
    if (n < 2)
        return (seq_t)0;

    /* Static cache: array of computed a(k) values, indexed by k */
    static seq_t *cache     = NULL;      /* array pointer */
    static size_t cache_cap = 0;         /* allocated capacity */
    static idx_t  cache_max = 0;         /* highest n that has been computed (inclusive) */

    /* Ensure that the array is large enough to hold index n */
    size_t required = (size_t)n + 1;     /* +1 to allow index n */
    if (required > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < required)
            new_cap = required;
        /* In C++ the cast is necessary */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;             /* allocation failed; fallback */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base values once */
    if (cache_max < 5) {
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)14;
        cache[5] = (seq_t)28;
        cache_max = 5;
    }

    /* Compute any new terms up to n using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* a(i) = 2 * a(i-1) – a(i-4) */
        seq_t twice = cache[(size_t)(i - 1)];
        twice = twice + twice;            /* 2 * a(i-1) */
        twice = twice - cache[(size_t)(i - 4)];
        cache[(size_t)i] = twice;
    }
    if (n > cache_max)
        cache_max = n;

    return cache[(size_t)n];
}
