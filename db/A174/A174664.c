/*
 * A174664  a(0)=1, a(1)=2, and a(n)=a(abs(floor((1/3)*n-a(n-2)))) for n>=2.
 * https://oeis.org/A174664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 776, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(n)=a(abs((n/3)-a(n-2))) for n>=2 */
#include <stdlib.h>

static seq_t *cache = NULL;   /* memoization array */
static idx_t cache_size = 0;  /* number of values currently stored */

/* Ensure all terms up to n are computed and cached */
static void ensure_cache(idx_t n)
{
    /* Allocate or enlarge the cache as needed */
    if (cache == NULL) {
        cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (cache == NULL) return; /* allocation failed – library will not use result */
    } else if (cache_size <= n) {
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (cache == NULL) return; /* allocation failed – library will not use result */
    }

    /* Base cases */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }
    if (cache_size == 1 && n >= 1) {
        cache[1] = (seq_t)2;
        cache_size = 2;
    }

    /* Compute missing entries in ascending order */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)2;
        } else {
            seq_t third   = (seq_t)(i / 3);   /* floor((1/3)*i) */
            seq_t prev    = cache[i - 2];
            seq_t idxdiff;

            /* Compute abs((i/3)-a(i-2)) without signed arithmetic */
            if (prev <= third)
                idxdiff = third - prev;
            else
                idxdiff = prev - third;

            idx_t k = (idx_t)idxdiff;  /* index into cache */
            cache[i] = cache[k];
        }
    }

    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
