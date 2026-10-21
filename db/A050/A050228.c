/*
 * A050228  a(n) is the number of subsequences {s(k)} of {1,2,3,...n} such
 *          that s(k+1)-s(k) is 1 or 3.
 * https://oeis.org/A050228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 2*a(n-4) + a(n-5), with
 *            initial values a(1)=1, a(2)=3, a(3)=6, a(4)=11, a(5)=19
 * Range:     u8 n <= 10, u16 n <= 25, u32 n <= 54, u64 n <= 112,
 *            u128 n <= 228, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values; index 0 holds a(1). */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Compute a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    /* First call: set up the five base terms. */
    if (cache == NULL) {
        cache = (seq_t *)calloc(5, sizeof(seq_t));
        if (cache == NULL) return 0;                     /* allocation failed */
        cache[0] = 1;  /* a(1) */
        cache[1] = 3;  /* a(2) */
        cache[2] = 6;  /* a(3) */
        cache[3] = 11; /* a(4) */
        cache[4] = 19; /* a(5) */
        cache_len = 5;
    }

    /* If the requested term is already cached, return it. */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Grow the cache to hold up to n terms. */
    size_t new_len = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (new_cache == NULL)          /* allocation failed */
        return 0;
    cache = new_cache;

    /* Compute missing terms using the linear recurrence. */
    for (size_t i = cache_len; i < new_len; ++i) {
        /* i corresponds to a(i+1). For i ≥ 5 the indices below are valid. */
        seq_t t1 = cache[i - 1] * (seq_t)3;          /* 3*a(n-1) */
        seq_t t2 = cache[i - 3] * (seq_t)2;          /* 2*a(n-3) */
        seq_t t3 = cache[i - 5];                    /* a(n-5) */
        seq_t pos = t1 + t2 + t3;                   /* 3*a(n-1) + 2*a(n-3) + a(n-5) */

        seq_t s1 = cache[i - 2] * (seq_t)3;          /* 3*a(n-2) */
        seq_t s2 = cache[i - 4] * (seq_t)2;          /* 2*a(n-4) */
        seq_t neg = s1 + s2;                        /* 3*a(n-2) + 2*a(n-4) */

        cache[i] = pos - neg;                       /* the recurrence */
    }

    cache_len = new_len;
    return cache[n - 1];
}
