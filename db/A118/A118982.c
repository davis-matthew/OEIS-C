/*
 * A118982  a(1)=1. For m>=0 and 1<=k<=2^m, a(2^m +k) = number of earlier
 *          terms of the sequence that are coprime to k.
 * https://oeis.org/A118982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   gcd, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A118982
   a(1) = 1.
   For m ≥ 0 and 1 ≤ k ≤ 2^m,
       a(2^m + k) = |{ j < 2^m + k : gcd(a(j), k) = 1 }|   */

#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache     = NULL;   /* cache[0] unused, indices 1…cache_len */
static size_t cache_len = 0;      /* highest index that is already known */
static size_t cache_cap = 0;      /* size of allocated array */

/* ensure that cache can hold index n (i.e. cache has at least n+1 slots) */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < (size_t)n + 1)
        new_cap <<= 1;

    seq_t *tmp = seq_alloc_array(new_cap);          /* allocates constructed objects */
    if (!tmp) abort();

    /* copy existing values */
    for (idx_t i = 1; i <= cache_len; ++i)
        tmp[i] = cache[i];

    seq_free_array(cache);
    cache     = tmp;
    cache_cap = new_cap;
}

/* floor(log2(v)) for v>0; v is idx_t (uint64_t) */
static unsigned int floor_log2_u64(idx_t v)
{
    /* v is never 0 here */
    return 63 - __builtin_clzll(v);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* undefined, but defensive */

    ensure_cache(n);

    /* initialise the very first term */
    if (cache_len == 0) {
        cache[1] = (seq_t)1;
        cache_len = 1;
    }

    if (n <= cache_len)
        return cache[n];

    /* compute all missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* i = 2^m + k  with 1 ≤ k ≤ 2^m  and i>1  */
        unsigned int m   = floor_log2_u64(i - 1);
        seq_t       k    = (seq_t)(i - ((idx_t)1 << m));
        seq_t       cnt  = 0ULL;

        for (idx_t j = 1; j < i; ++j) {
            if (gcd(cache[j], k) == (seq_t)1)
                ++cnt;
        }
        cache[i] = cnt;
    }
    cache_len = n;
    return cache[n];
}
