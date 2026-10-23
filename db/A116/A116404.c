/*
 * A116404  Expansion of (1-x)/((1-x)^2 - x^2*(1+x)^2).
 * https://oeis.org/A116404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-3) + a(n-4) Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=6
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, calloc, realloc, abort */

/* Cached sequence values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure the cache can hold indices up to n.
   New elements are initialized to zero. */
static void ensure_cache(idx_t n)
{
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)(n + 1));
        if (!tmp && n + 1 > 0) abort();
        cache = tmp;
        /* zero initialise new slots (safe for built‑in and arbitrary types) */
        for (idx_t i = cache_size; i <= n; ++i) cache[i] = 0;
        cache_size = n + 1;
    }
}

/* Initialise the first four terms if not yet done. */
static void init_cache(void)
{
    if (!cache) {
        cache = (seq_t *)calloc(4, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache_size = 4;
    }
}

/* Return a(n) for the OEIS sequence A116404. */
seq_t a(idx_t n)
{
    init_cache();                     /* make sure base values exist */

    if (n < cache_size) return cache[n];

    idx_t start = cache_size;          /* first index to compute */
    ensure_cache(n);                   /* grow cache if necessary */

    for (idx_t i = start; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + 2*a(i-3) + a(i-4) */
        cache[i] = (seq_t)2 * cache[i - 1] + (seq_t)2 * cache[i - 3] + cache[i - 4];
    }
    return cache[n];
}
