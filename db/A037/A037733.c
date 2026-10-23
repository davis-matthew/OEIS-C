/*
 * A037733  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,0,3.
 * https://oeis.org/A037733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-1) + a(n-4) - 7*a(n-5)
   Initial values: a(1)=2, a(2)=15, a(3)=105, a(4)=738, a(5)=5168 */
#ifdef __cplusplus
#include <new>
#else
#include <stdlib.h>
#endif

/* Static cache for memoization */
static seq_t *cache = NULL;     /* 1‑based array of results */
static size_t cache_size = 0;   /* allocated size (elements, not bytes) */
static idx_t  computed_max = 0; /* largest n for which cache[n] is valid */

/* Ensure the cache can hold up to desired_n indices */
static void grow_cache(idx_t desired_n)
{
    if (cache_size > desired_n) return;
    size_t new_cap = (size_t)desired_n + 1; /* +1 because we use 1‑based indexing */
#ifdef __cplusplus
    seq_t *new_ptr = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cache_size; ++i)
            new_ptr[i] = cache[i];
        delete [] cache;
    }
    cache = new_ptr;
#else
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    cache = new_ptr;
#endif
    cache_size = new_cap;
}

/* The sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, return 0 */

    /* Make sure the cache is allocated and big enough */
    if (!cache) grow_cache(5);
    if (n >= (idx_t)cache_size) grow_cache(n);

    /* Initialize initial terms if not yet done */
    if (computed_max < 5) {
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)15;
        cache[3] = (seq_t)105;
        cache[4] = (seq_t)738;
        cache[5] = (seq_t)5168;
        computed_max = 5;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        cache[i] = (seq_t)7 * cache[i-1] + cache[i-4] - (seq_t)7 * cache[i-5];
    }
    computed_max = n;

    return cache[n];
}
