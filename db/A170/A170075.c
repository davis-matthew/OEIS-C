/*
 * A170075  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 36: a(n) = 2 + 16 · Σ_{k=0}^{n-1} a(k) For
 *            n = 37: a(37) = 1 + 16 · Σ_{k=1}^{36} a(k) – 136 · a(0) For n ≥
 *            38: a(n) = 16 · Σ_{k=n-36}^{n-1} a(k) – 136 · a(n-37)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cached sequence and its prefix sums. */
static seq_t *seq_cache = NULL;
static seq_t *pref_cache = NULL;
static size_t  cache_cap = 0;     /* currently allocated capacity */
static size_t  cache_len = 0;     /* number of terms already computed (cache_len = last_index+1) */

/* Ensure that the caches can hold indices up to n.  Grow capacity by doubling. */
static void ensure_caches(size_t n)
{
    if (cache_cap > n)
        return;
    size_t new_cap = cache_cap == 0 ? 1 : cache_cap;
    while (new_cap <= n)
        new_cap *= 2;
    seq_cache   = (seq_t *)realloc(seq_cache,   new_cap * sizeof(seq_t));
    pref_cache  = (seq_t *)realloc(pref_cache,  new_cap * sizeof(seq_t));
    /* New entries are implicitly zero because a(n)=0 for n<0 in the recurrence. */
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Allocate enough space for indices up to n. */
    ensure_caches((size_t)n);

    /* Compute missing terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {                     /* a(0) = 1 */
            seq_cache[0] = 1;
            pref_cache[0] = 1;
        } else if (i <= 36) {              /* a(i) = 2 + 16 * Σ_{k=0}^{i-1} a(k) */
            seq_t sum = pref_cache[i-1];   /* prefix sum up to i-1 */
            seq_cache[i] = 2 + sum * 16;
            pref_cache[i] = pref_cache[i-1] + seq_cache[i];
        } else if (i == 37) {              /* a(37) = 1 + 16 * Σ_{k=1}^{36} a(k) – 136·a(0) */
            seq_t sum = pref_cache[36] - pref_cache[0];   /* Σ_{k=1}^{36} a(k) */
            seq_cache[i] = 1 + sum * 16 - 136 * seq_cache[0];
            pref_cache[i] = pref_cache[36] + seq_cache[i];
        } else {                            /* i ≥ 38: a(i) = 16·Σ_{k=i-36}^{i-1} a(k) – 136·a(i-37) */
            seq_t sum = pref_cache[i-1] - pref_cache[i-37];  /* Σ_{k=i-36}^{i-1} a(k) */
            seq_cache[i] = sum * 16 - 136 * seq_cache[i-37];
            pref_cache[i] = pref_cache[i-1] + seq_cache[i];
        }
    }

    cache_len = n + 1;
    return seq_cache[n];
}
