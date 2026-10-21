/*
 * A050047  a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 2 - 2^(p+1) and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1 and a(2) = a(3) = 2.
 * https://oeis.org/A050047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 126, u64 n <= 505,
 *            u128 n <= 3084, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050047
   a(1)  = 1
   a(2)  = a(3) = 2
   For n ≥ 4:
      let p = floor(log2(n-2))
      let m = 2*n - 2 - 2^(p+1)
      a(n) = a(n-1) + a(m)
*/

#include <stdlib.h>
#include <string.h>

/* External integer types supplied by the build */
extern seq_t a(idx_t);

/* ------------------------------------------------------------------ */
/*  Shared caching data                                               */
/* ------------------------------------------------------------------ */
static seq_t *cache      = NULL;   /* 1‑based: cache[1] = a(1)… */
static idx_t  max_computed = 0;    /* highest n with a stored value  */
static size_t cache_size   = 0;    /* allocated slots (≥ max_computed+1) */

/* ------------------------------------------------------------------ */
/*  Utility to grow the cache                                          */
/* ------------------------------------------------------------------ */
static void
ensure_capacity(idx_t n)
{
    /* need at least indices 0..n inclusive */
    size_t needed = (size_t)n + 1;
    if (needed <= cache_size)
        return;

    /* double until big enough */
    size_t new_size = cache_size ? cache_size : 4;
    while (new_size < needed)
        new_size <<= 1;

    seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
    if (!new_cache)
        abort();   /* out of memory */

    /* copy existing data */
    if (cache)
    {
        memcpy(new_cache, cache, (size_t)(max_computed + 1) * sizeof(seq_t));
        free(cache);
    }

    cache      = new_cache;
    cache_size = new_size;
}

/* ------------------------------------------------------------------ */
/*  Initialize base cases                                               */
/* ------------------------------------------------------------------ */
static void
init_cache(void)
{
    if (max_computed != 0)
        return;                   /* already done */

    ensure_capacity(3);
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)2;
    max_computed = 3;
}

/* ------------------------------------------------------------------ */
/*  floor(log2(x)) for x ≥ 1, using idx_t                              */
/* ------------------------------------------------------------------ */
static inline idx_t
floor_log2_idx(idx_t x)
{
    idx_t r = 0;
    while (x >>= 1)
        ++r;
    return r;
}

/* ------------------------------------------------------------------ */
/*  Public function a(idx_t n)                                         */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined by OEIS, treat as 0 */

    init_cache();

    if (n <= max_computed)
        return cache[n];

    /* grow cache to hold up to n */
    ensure_capacity(n);

    /* compute missing terms */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        /* find p with 2^p < i-1 <= 2^(p+1)  -> p = floor(log2(i-2))  */
        idx_t p = floor_log2_idx(i - 2);
        idx_t pow2 = ((idx_t)1) << (p + 1);
        idx_t m = 2 * i - 2 - pow2;          /* 1 ≤ m < i */
        cache[i] = cache[i - 1] + cache[m];
    }

    max_computed = n;
    return cache[n];
}
