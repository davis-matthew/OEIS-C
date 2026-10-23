/*
 * A152298  a(n) = (3^n-1)/2 if n odd, (3^n-1)/8 if n even.
 * https://oeis.org/A152298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n-1)/(2^(3-2*Mod[n,2])) Recurrence: a(n) = 10*a(n-2) -
 *            9*a(n-4) Initial values: a(0)=0, a(1)=1, a(2)=1, a(3)=13
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* current capacity (bytes) */
    static size_t cache_used = 0;  /* number of initialized entries */

    /* Lazy initialization of the first four terms */
    if (cache == NULL) {
        cache_cap = 8;               /* arbitrary initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out of memory: return 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)13;
        cache_used = 4;
    }

    /* Return cached value if already computed */
    if ((size_t)n < cache_used)
        return cache[n];

    /* Grow the cache if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < needed) new_cap = needed;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out of memory */
        cache_cap = new_cap;
    }

    /* Compute missing values up to n using the recurrence */
    for (size_t i = cache_used; i < needed; ++i) {
        /* For i >= 4: a(i) = 10*a(i-2) - 9*a(i-4) */
        seq_t term1 = cache[i-2];
        seq_t term2 = cache[i-4];
        cache[i] = ((seq_t)10 * term1) - ((seq_t)9 * term2);
    }
    cache_used = needed;

    return cache[n];
}
