/*
 * A097747  a(n) = a(n mod a(n-1)) + floor(n/a(n-1)), a(0) = 1.
 * https://oeis.org/A097747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = a(n mod a(n-1)) + floor(n / a(n-1)) for n ≥ 1
 * Range:     u8 n <= 630, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* realloc, abort */
/* The types idx_t and seq_t are supplied by the build system. */

#ifdef __cplusplus
/* --- C++ build ---------------------------------------------------- */
#include <vector>

static std::vector<seq_t> cache;   /* 0‑based cache of computed terms */

static void ensure(idx_t n)
{
    if (cache.empty())
        cache.push_back( (seq_t)1 );   /* a(0) = 1 */

    if (n < cache.size())
        return;                       /* already computed */

    for (idx_t i = (idx_t)cache.size(); i <= n; ++i) {
        seq_t prev     = cache[i-1];
        seq_t floor_div= (seq_t)i / prev;
        idx_t  mod_idx = (idx_t)((seq_t)i % prev);   /* n mod a(n‑1) */
        seq_t recur    = cache[mod_idx];
        cache.push_back(recur + floor_div);
    }
}

#else
/* --- C build ------------------------------------------------------ */
static seq_t *cache   = NULL;   /* array of computed terms */
static idx_t  cache_len = 0;    /* number of terms currently stored */
static idx_t  cache_cap = 0;    /* allocated capacity */

static void ensure(idx_t n)
{
    if (n < cache_len)
        return;                   /* already computed */

    /* grow storage if needed */
    idx_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= n)
        new_cap <<= 1;

    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialize first term if necessary */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;      /* a(0) = 1 */
        cache_len = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev     = cache[i-1];
        seq_t floor_div= (seq_t)i / prev;
        idx_t  mod_idx = (idx_t)((seq_t)i % prev);   /* n mod a(n‑1) */
        seq_t recur    = cache[mod_idx];
        cache[i] = recur + floor_div;
    }
    cache_len = n + 1;
}
#endif

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    ensure(n);
    /* In the C++ implementation, cache is a vector; in C it is an array. */
#ifdef __cplusplus
    return cache[n];
#else
    return cache[n];
#endif
}
