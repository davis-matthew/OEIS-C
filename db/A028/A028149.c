/*
 * A028149  Expansion of 1/((1-4x)(1-7x)(1-9x)(1-10x)).
 * https://oeis.org/A028149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*10^(n+3)-9^(n+4)+5*7^(n+3)-4^(n+3))/90
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) from the generating function 1/((1-4x)(1-7x)(1-9x)(1-10x))
   using the linear recurrence:
     a(0)=1, a(1)=30, a(2)=573, a(3)=8902,
     a(n) = 30*a(n-1) - 327*a(n-2) + 1522*a(n-3) - 2520*a(n-4)   for n≥4.
   Memoisation is used so that any order of calls is efficient.  Only
   unsigned arithmetic of type seq_t is used; all intermediate results
   fit into seq_t for all terms that are tested by the caller.  No
   floating point or external libraries are required. */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t last_computed = 0;   /* index of highest value computed in cache */

static void ensure_cache(idx_t n)
{
    /* Allocate / grow the cache if necessary.  Initial fill the first
       four terms – afterwards values will be computed on demand. */
    if (cache == NULL) {
        size_t cap = (size_t)4;
        if ((idx_t)(cap) <= n)
            cap = (size_t)(n + 1);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache_size = (idx_t)cap;
        cache[0] = 1;
        cache[1] = 30;
        cache[2] = 573;
        cache[3] = 8902;
        for (idx_t i = 4; i < cache_size; ++i)
            cache[i] = 0;
        last_computed = 3;
    } else if (n >= cache_size) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Realloc failed – in a real program you would handle this,
               but for the test harness we can simply abort. */
            abort();
        }
        cache = tmp;
        for (idx_t i = cache_size; i < new_cap; ++i)
            cache[i] = 0;
        cache_size = (idx_t)new_cap;
    }

    /* Compute any new terms up to n. */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t a_nm1 = cache[i - 1];
            seq_t a_nm2 = cache[i - 2];
            seq_t a_nm3 = cache[i - 3];
            seq_t a_nm4 = cache[i - 4];
            seq_t large  = a_nm1 * (seq_t)30 + a_nm3 * (seq_t)1522;
            seq_t small  = a_nm2 * (seq_t)327 + a_nm4 * (seq_t)2520;
            cache[i] = large - small;    /* large ≥ small for all n≥4 */
        }
        last_computed = n;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
