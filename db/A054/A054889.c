/*
 * A054889  Layer counting sequence for hyperbolic tessellation by regular
 *          pentagons of angle 2*Pi/5.
 * https://oeis.org/A054889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A054889 – Layer counting sequence for the hyperbolic tessellation by regular pentagons.
   Recurrence: a(1)=1, a(2)=5, a(3)=20, a(4)=70, a(5)=245
                a(n)=3*a(n-1)+a(n-2)+3*a(n-3)-a(n-4)   for n≥6   */
/* Requires only the elementary arithmetic operators and realloc.  */

#include <stdlib.h>

static seq_t *cache   = NULL;    /* cache[0] unused, cache[1] holds a(1) */
static size_t cache_len = 0;     /* highest index that has been computed */
static size_t cache_cap = 0;     /* allocated capacity (number of elements) */

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for the offset, but safe */
        return (seq_t)0;

    /* Initialize the cache if it does not yet exist. */
    if (cache == NULL) {
        cache_cap = 8;                 /* start with a few slots */
        cache     = (seq_t *)malloc(cache_cap * sizeof(*cache));
        if (cache == NULL)            /* allocation failed */
            return (seq_t)0;
    }

    /* Ensure the array is large enough for index n. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
        if (tmp == NULL)            /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Store the first five terms, if not already present. */
    if (cache_len < 5) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)20;
        cache[4] = (seq_t)70;
        cache[5] = (seq_t)245;
        cache_len = 5;
    }

    /* Return a(n) for n ≤ 5. */
    if (n <= 5)
        return cache[n];

    /* Compute any missing terms up to n using the recurrence. */
    if ((size_t)n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t t1 = (seq_t)3 * cache[i-1];
            seq_t t2 =              cache[i-2];
            seq_t t3 = (seq_t)3 * cache[i-3];
            seq_t t4 =              cache[i-4];
            cache[i] = t1 + t2 + t3 - t4;
        }
        cache_len = (size_t)n;
    }

    return cache[n];
}
