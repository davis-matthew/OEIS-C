/*
 * A199928  Trisection 1 of A199802.
 * https://oeis.org/A199928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - 5*a(n-2) + a(n-3) - a(n-4), a(0)=2, a(1)=-1,
 *            a(2)=-8, a(3)=15
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 26, i64 n <= 53, i128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* dynamic cache for the recurrence */
static seq_t *cache = NULL;
static size_t cap_size = 0;   /* allocated size of cache */
static size_t num_elems = 0;  /* number of elements already computed */

/* helper to grow the cache to at least n+1 elements */
static void grow_cache_to(idx_t n)
{
    /* If cache not yet allocated, start with 4 */
    if (cap_size == 0) {
        cap_size = 4;
        cache = (seq_t *)malloc(cap_size * sizeof(seq_t));
        cache[0] = (seq_t) 2;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)-8;
        cache[3] = (seq_t)15;
        num_elems = 4;
    }

    /* If we already have enough space, nothing to do */
    if ((idx_t)cap_size > n) return;

    /* Double capacity until it covers n */
    size_t new_cap = cap_size;
    while ((idx_t)new_cap <= n) new_cap <<= 1;

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cap_size = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 4) {
        /* handle the first four terms directly */
        switch (n) {
            case 0: return (seq_t) 2;
            case 1: return (seq_t)-1;
            case 2: return (seq_t)-8;
            case 3: return (seq_t)15;
        }
    }

    /* ensure cache has enough space and initial values */
    grow_cache_to(n);

    /* compute missing terms on demand */
    for (size_t i = num_elems; i <= (size_t)n; ++i) {
        cache[i] = -cache[i-1] - (seq_t)5 * cache[i-2]
                    + cache[i-3] - cache[i-4];
    }
    num_elems = (size_t)n + 1;

    return cache[n];
}
