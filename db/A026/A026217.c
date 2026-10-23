/*
 * A026217  Duplicate of A026178.
 * https://oeis.org/A026217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 63, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A026217 (duplicate of A026178):
   a(1)=1.
   For n>1:
      if n is odd:    a(n) = (3*n-1)/2;
      else (n=2*k):
        if the value k has not appeared among a(1)..a(n-1),
             a(n) = k;
        else
             a(n) = 3*k.                          */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Global cache for memoisation */
static seq_t *cache      = NULL;   /* cache[0] unused; cache[i] = a(i) */
static size_t cache_cap  = 0;
static size_t cache_sz   = 0;       /* largest index computed */

/* Array of flags indicating whether a value has already appeared in the perm */
static bool *used        = NULL;   /* used[x] == true iff x has appeared so far */
static size_t used_cap   = 0;

/* Ensure cache is large enough for indices up to n */
static void ensure_cache_capacity(size_t n)
{
    if (cache_cap <= n) {
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache   = tmp;
        cache_cap = new_cap;
    }
}

/* Ensure used[] is large enough for indices up to n */
static void ensure_used_capacity(size_t n)
{
    if (used_cap <= n) {
        size_t new_cap = (n + 1) * 2;
        bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
        if (!tmp) abort();
        /* initialise new entries to false */
        for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
        used     = tmp;
        used_cap = new_cap;
    }
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 as a safe value */

    /* Initialise global storage on first call. */
    if (cache_sz == 0) {
        cache_cap = 4;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();

        used_cap = 4;
        used = (bool *)calloc(used_cap, sizeof(bool));
        if (!used) abort();
    }

    /* If already computed up to n, just return it. */
    if ((size_t)n <= cache_sz) {
        return cache[n];
    }

    /* Expand cache and used arrays as needed. */
    ensure_cache_capacity((size_t)n);
    ensure_used_capacity((size_t)n);

    /* Compute values from cache_sz+1 up to n. */
    for (idx_t i = cache_sz + 1; i <= n; ++i) {
        seq_t val;
        if ((i & 1) != 0) {                  /* odd i: a(i) = (3*i-1)/2 */
            val = ((seq_t)i * 3 - 1) / 2;
            ensure_used_capacity((size_t)val);
            used[(size_t)val] = true;
            cache[(size_t)i] = val;
        } else {                              /* even i = 2*k */
            idx_t k = i >> 1;                 /* k = i/2 */
            size_t k_idx = (size_t)k;
            ensure_used_capacity(3 * k_idx);
            if (!used[k_idx]) {
                val = (seq_t)k;
            } else {
                val = (seq_t)(3 * k);
            }
            ensure_used_capacity((size_t)val);
            used[(size_t)val] = true;
            cache[(size_t)i] = val;
        }
    }
    cache_sz = (size_t)n;
    return cache[(size_t)n];
}
