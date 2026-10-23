/*
 * A153316  Numerators of continued fraction convergents to sqrt(5/4).
 * https://oeis.org/A153316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=9; for n>=2: if n even a(n)=2*a(n-1)+a(n-2); if n
 *            odd a(n)=8*a(n-1)+a(n-2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;           /* cached terms */
static idx_t  cache_size = 0;        /* allocated cache size */
static idx_t  last_computed = 0;     /* highest index that has been computed */
static bool   initialized = false;   /* have we set the base values? */

/* Ensure the cache can store terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(1);                /* allocation failed */
    cache = tmp;
    cache_size = new_size;
}

/* Return the n-th term of A153316. */
seq_t a(idx_t n)
{
    if (!initialized)
    {
        /* Allocate space for the first four terms. */
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = 1;   /* a(0) */
        cache[1] = 9;   /* a(1) */
        cache[2] = 19;  /* a(2) */
        cache[3] = 161; /* a(3) */
        cache_size = 4;
        last_computed = 3;
        initialized = true;
    }

    if (n <= last_computed) return cache[n];

    /* Extend the cache if necessary and compute missing terms. */
    ensure_cache(n);
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        if ((i & 1) == 0)          /* even index */
            cache[i] = 2 * cache[i-1] + cache[i-2];
        else                       /* odd index */
            cache[i] = 8 * cache[i-1] + cache[i-2];
    }
    last_computed = n;
    return cache[n];
}
