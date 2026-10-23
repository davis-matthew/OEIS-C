/*
 * A126443  a(n) = Sum_{k=0..n-1} C(n-1,k)*a(k)*2^k for n>0, with a(0)=1.
 * https://oeis.org/A126443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} C(n-1,k)*a(k)*2^k (a(0)=1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required by the build system: 
   idx_t – unsigned integer index type (e.g., uint64_t)
   seq_t – unsigned integer or arbitrary‑precision integer type   */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, calloc, realloc, free, abort */

static seq_t *cache = NULL;   /* cache of previously computed values */
static size_t cache_cap = 0;  /* current allocated capacity of cache */

/* Grow the cache so that it can hold indices up to n (inclusive). */
static void grow_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (needed <= cache_cap)
        return;

    size_t new_cap = needed * 2;
    seq_t *new_cache;

#ifdef __cplusplus
    /* Value‑initialize the new array (zero for built‑in types). */
    new_cache = new seq_t[new_cap]();
#else
    new_cache = (seq_t*)calloc(new_cap, sizeof(seq_t));
#endif

    /* Copy any existing values. */
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute all a(k) for k ≤ n and store them in cache. */
static void compute_up_to(idx_t n)
{
    /* Initialise the cache if this is the first call. */
    if (cache == NULL) {
#ifdef __cplusplus
        cache = new seq_t[1]();   /* a(0) will be set next */
#else
        cache = (seq_t*)calloc(1, sizeof(seq_t));
#endif
        cache_cap = 1;
        cache[0] = 1;            /* a(0) = 1 */
    }

    if ((idx_t)(cache_cap - 1) < n)
        grow_cache(n);

    for (idx_t i = 1; i <= n; ++i) {
        /* Skip if already computed (cache entry is non‑zero). */
        if (cache[i] != 0)
            continue;

        seq_t sum   = 0;
        seq_t binom = 1;   /* C(i-1,0)        */
        seq_t pow2k = 1;   /* 2^0             */

        for (idx_t k = 0; k < i; ++k) {
            seq_t term = binom * cache[k];
            term *= pow2k;
            sum += term;

            if (k + 1 < i) {
                /* Update binomial coefficient to C(i-1,k+1). */
                seq_t num = binom * (seq_t)(i - 1 - k);
                seq_t den = (seq_t)(k + 1);
                binom = num / den;   /* exact division */

                pow2k *= 2;           /* multiply by 2 */
            }
        }

        cache[i] = sum;
    }
}

/* Public entry point required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                 /* a(0) = 1 */
    compute_up_to(n);
    return cache[n];
}
