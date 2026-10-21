/*
 * A076218  Numbers n such that 2*n^2 - 3*n + 1 is a square.
 * https://oeis.org/A076218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3) (n >= 5) Initial values:
 *            a(1) = 0, a(2) = 1, a(3) = 5, a(4) = 145
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>            /* for bool */

#ifndef SEQ_T_DEFINED
/* The build supplies typedefs for seq_t and idx_t. */
#endif

/* Static cache for memoised values */
static seq_t *cache      = NULL;   /* 1‑based array, cache[1..N] */
static size_t cache_cap  = 0;      /* current allocated capacity */
static idx_t  cache_last = 0;      /* largest index already computed */
static bool   cache_init = false;  /* have we set the base values? */

/* Ensure the cache can store up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)(n + 1)) new_cap <<= 1;

        seq_t *tmp;
#ifdef __cplusplus
        /* For a non‑trivial type, allocate fresh array and copy. */
        tmp = new seq_t[new_cap];
        for (size_t i = 0; i < cache_cap; ++i)
            tmp[i] = cache[i];
        delete[] cache;
#else
        /* Plain‑integer types – realloc is fine. */
        tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) abort();          /* allocation failed */
        cache      = tmp;
        cache_cap  = new_cap;
    }
}

/* Initialise base values if not yet set */
static void init_base_values(void)
{
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)5;
    cache[4] = (seq_t)145;
    cache_last = 4;
    cache_init = true;
}

/* Main accessor function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset 1 – ignore n==0 */

    ensure_cache(n);

    if (!cache_init) init_base_values();

    if (n > cache_last) {
        idx_t start = cache_last + 1;
        if (start < 5) start = 5;          /* recurrence valid from n=5 */
        for (idx_t i = start; i <= n; ++i) {
            /* a(i) = 35*a(i-1) - 35*a(i-2) + a(i-3) */
            cache[i] = (seq_t)35 * cache[i-1]
                       - (seq_t)35 * cache[i-2]
                       + cache[i-3];
        }
        cache_last = n;
    }

    return cache[n];
}
