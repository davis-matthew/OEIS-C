/*
 * A050070  a(n) = a(n-1)+a(m), where m=n-1-2^p and 2^p<n-1<=2^(p+1), for m >=
 *          4.
 * https://oeis.org/A050070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 3 For n >= 3: a(n) = a(n-1) + a(m) where m =
 *            n-1 - 2^p p = floor(log2(n-2)) This follows the OEIS recurrence
 *            a(n) = a(n-1)+a(n-1-2^⌊log₂(n-2)⌋)
 * Range:     u8 n <= 29, u16 n <= 188, u32 n <= 1650, u64 n <= 8193,
 *            u128 n <= 8193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <new>
#endif

/* Ensure enough space for indices up to n (inclusive). */
static void ensure_cache(size_t n)
{
    static size_t cache_cap = 0;
    static seq_t *cache = NULL;
    if (n <= cache_cap) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < n) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!new_cache) abort();            /* Out of memory */
#ifdef __cplusplus
    /* Construct newly allocated objects for a class type, no-op for integral types. */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new (new_cache + i) seq_t();
#endif
    cache = new_cache;
    cache_cap = new_cap;
    /* Store pointers in static variables for use in a() */
    /* Use a lambda to capture by reference */
    /* However, we need to expose cache and other static variables to a() */
    /* We'll declare them static outside this function. */
}

/* We need to expose cache, cache_cap, last_computed globally for a(). */
static seq_t *global_cache = NULL;
static size_t global_cache_cap = 0;
static idx_t last_computed = 0;

static void init(void)
{
    if (global_cache) return;   /* already initialized */
    /* Allocate at least space for indices 0..3 (0 unused). */
    size_t init_cap = 4;
    global_cache = (seq_t *)malloc(sizeof(seq_t) * init_cap);
    if (!global_cache) abort();
    global_cache_cap = init_cap;
#ifdef __cplusplus
    /* Default construct if needed */
    for (size_t i = 0; i < init_cap; ++i)
        new (global_cache + i) seq_t();
#endif
    /* Set base values */
    global_cache[1] = (seq_t)1;
    global_cache[2] = (seq_t)3;
    last_computed = 2;
}

/* Expand the global cache to at least n+1 in size. */
static void grow_cache(size_t n)
{
    if (n + 1 <= global_cache_cap) return;
    size_t new_cap = global_cache_cap ? (global_cache_cap << 1) : 16;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(global_cache, sizeof(seq_t) * new_cap);
    if (!new_cache) abort();
#ifdef __cplusplus
    /* Construct new elements */
    for (size_t i = global_cache_cap; i < new_cap; ++i)
        new (new_cache + i) seq_t();
#endif
    global_cache = new_cache;
    global_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Lazy initialization */
    if (!global_cache) init();

    if (n <= last_computed)
        return global_cache[n];

    /* Compute new terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* For i >= 3 we can use the recurrence, for i==3 it also works */
        idx_t temp = i - 2;                 /* i-2 >= 1 for i>=3 */
        /* Find largest p such that 2^p <= temp (actually 2^p < i-1). */
        int p = 63 - __builtin_clzll((unsigned long long)temp);
        seq_t pow2 = (seq_t)( (seq_t)1 << p );
        idx_t m = (i - 1) - (idx_t)pow2;    /* m >= 1 */
        seq_t val = global_cache[i - 1] + global_cache[m];
        grow_cache(i);                       /* Ensure we have space for index i */
        global_cache[i] = val;
    }
    last_computed = n;
    return global_cache[n];
}
