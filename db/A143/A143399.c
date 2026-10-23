/*
 * A143399  Expansion of x^k/Product_{t=k..2k} (1-tx) for k=4.
 * https://oeis.org/A143399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A143399:
   a(n) = 30*a(n-1) - 355*a(n-2) + 2070*a(n-3) - 5944*a(n-4) + 6720*a(n-5)
   Initial values:
   a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* recurrence coefficients */
static const seq_t C30   = 30;
static const seq_t C355  = 355;
static const seq_t C2070 = 2070;
static const seq_t C5944 = 5944;
static const seq_t C6720 = 6720;

#ifdef __cplusplus
/* C++ implementation using std::vector for automatic construction
   and deallocation. */
static std::vector<seq_t> cache_vec;

/* Ensure cache contains values up to n */
static void ensure_cache(idx_t n)
{
    size_t old = cache_vec.size();
    if (old > n) return;            /* already computed */

    cache_vec.resize((size_t)n + 1); /* extend vector, value‑initialized */

    /* set a(4) if it has not yet been set */
    if (n >= 4 && old <= 4) cache_vec[4] = 1;

    /* compute new terms (recurrence is valid for i >= 5) */
    for (idx_t i = old; i <= n; ++i)
    {
        if (i < 5) continue;    /* base cases already initialized */
        seq_t left  = C30 * cache_vec[i-1] + C2070 * cache_vec[i-3] + C6720 * cache_vec[i-5];
        seq_t right = C355 * cache_vec[i-2] + C5944 * cache_vec[i-4];
        cache_vec[i] = left - right;
    }
}
#else
/* C implementation using malloc/realloc */
static seq_t *cache = NULL;
static idx_t cached_len = 0;

static void ensure_cache(idx_t n)
{
    if (n < cached_len) return;          /* already computed */

    size_t new_len = (size_t)n + 1U;      /* number of entries needed */

    if (!cache) {
        /* first allocation */
        cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!cache) exit(1);             /* allocation failure */
        for (size_t i = 0; i < new_len; ++i) cache[i] = 0;
        if (new_len > 4) cache[4] = 1;   /* a(4) = 1 */
        for (idx_t i = 5; i <= n; ++i) {
            seq_t left  = C30 * cache[i-1] + C2070 * cache[i-3] + C6720 * cache[i-5];
            seq_t right = C355 * cache[i-2] + C5944 * cache[i-4];
            cache[i] = left - right;
        }
    } else {
        /* grow the allocation */
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!cache) exit(1);             /* allocation failure */
        for (size_t i = cached_len; i < new_len; ++i) cache[i] = 0;
        if (cached_len <= 4 && new_len > 4) cache[4] = 1;
        for (idx_t i = cached_len; i <= n; ++i) {
            if (i < 5) continue;        /* base cases already set */
            seq_t left  = C30 * cache[i-1] + C2070 * cache[i-3] + C6720 * cache[i-5];
            seq_t right = C355 * cache[i-2] + C5944 * cache[i-4];
            cache[i] = left - right;
        }
    }
    cached_len = new_len;
}
#endif

/* external linkage required by the build system */
seq_t a(idx_t n)
{
#ifdef __cplusplus
    ensure_cache(n);
    return cache_vec[(size_t)n];
#else
    ensure_cache(n);
    return cache[n];
#endif
}
