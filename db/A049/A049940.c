/*
 * A049940  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 3, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1) with a(1) = a(2) = 1.
 * https://oeis.org/A049940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1) + a(2) + … + a(n‑1) + a(m), with a(1) = a(2) = 1, m
 *            = 2*n – 3 – 2^ceil(log₂(n‑1)) for n ≥ 3
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>

/* Static caches for a(n) and its prefix sums.
 * Index 0 is unused so that a(1) is at a_cache[1], etc. */
static seq_t *a_cache      = NULL;  /* a(n)            */
static seq_t *prefix_cache = NULL;  /* prefix sums   */
static idx_t  cache_cap    = 0;     /* allocated size (≥ highest requested) */
static idx_t  cache_size   = 0;     /* highest index whose value is computed */

/* Grow the caches to at least `need` elements.  All allocations use
 * explicit casts to satisfy C++ (realloc returns void*). */
static void grow_cache(idx_t need)
{
    if (cache_cap >= need)
        return;

    /* start with a small capacity if first time */
    if (cache_cap == 0)
        cache_cap = 8;
    while (cache_cap < need)
        cache_cap <<= 1;          /* double until large enough */

    a_cache      = (seq_t *)realloc(a_cache,      cache_cap * sizeof(seq_t));
    prefix_cache = (seq_t *)realloc(prefix_cache, cache_cap * sizeof(seq_t));
    if (a_cache == NULL || prefix_cache == NULL) {
        abort();   /* allocation failed */
    }
}

/* Compute all terms up to `n` (n ≥ 3) using the recurrence and prefix
 * sums for efficiency.  Each new a(i) needs only a(i‑1) computed values. */
static void compute_up_to_(idx_t n)
{
    grow_cache(n + 1);          /* ensure space for index n */

    /* initialise base cases if not already done */
    if (cache_size == 0) {
        a_cache[0]      = (seq_t)0;
        prefix_cache[0] = (seq_t)0;
        a_cache[1]      = (seq_t)1;
        prefix_cache[1] = (seq_t)1;
        a_cache[2]      = (seq_t)1;
        prefix_cache[2] = (seq_t)2;
        cache_size = 2;
    }

    /* fill missing entries */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* ceil(log2(i-1)) – i‑1 ≥ 2 for i ≥ 3 */
        idx_t v = i - 1;
        unsigned fl = 63 - __builtin_clzll(v);               /* floor log2 */
        unsigned ceil = ((1ULL << fl) == v) ? fl : fl + 1;   /* ceil log2 */
        idx_t pow2 = ((idx_t)1) << ceil;                     /* 2^ceil */

        idx_t m = (idx_t)2 * i - 3 - pow2;                  /* m = 2i-3-2^ceil */

        seq_t sum_before = prefix_cache[i - 1];   /* Σ_{k=1}^{i-1} a(k) */
        seq_t a_m        = a_cache[m];             /* a(m)   */
        seq_t a_i        = sum_before + a_m;       /* a(i)   */
        seq_t new_prefix = sum_before + a_i;       /* cumulative sum up to i */

        a_cache[i]      = a_i;
        prefix_cache[i] = new_prefix;
    }
    cache_size = n;
}

/* Public function: returns a(n).  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined, return 0 */

    if (n <= 2)                          /* base cases */
        return (seq_t)1;

    if (cache_size < n)                  /* compute missing terms */
        compute_up_to_(n);

    return a_cache[n];
}
