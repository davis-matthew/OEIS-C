/*
 * A098709  a(n) = smallest positive multiple of (number of terms of {a(1),
 *          a(2), ..., a(n-1)} that are coprime to n) that is not among
 *          previous terms of sequence.
 * https://oeis.org/A098709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A098709:
 * a(n) = smallest positive multiple of
 *        C(n) = #{k | 1 ≤ k < n, gcd(a(k),n)=1}
 * that is not among {a(1),…,a(n-1)}.
 * a(1) = 1. */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>

/* GCD routine provided by the build system */
#include "../utils/gcd.h"   /* gcd(seq_t a, seq_t b) */

static seq_t *cache = NULL;     /* array holding the sequence values */
static idx_t  cache_size = 0;   /* number of stored terms */
static idx_t  cache_cap  = 0;   /* currently allocated capacity */

/* Ensure the cache has at least 'needed' elements */
static void ensure_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)                /* abort if out of memory */
        exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

/* Linear search: does 'val' appear among the first 'len' entries of cache? */
static inline bool contains(seq_t val, idx_t len)
{
    for (idx_t i = 0; i < len; ++i)
        if (cache[i] == val)
            return true;
    return false;
}

/* Compute all terms up to index 'n' (inclusive) */
static void compute_up_to(idx_t n)
{
    /* initialise a(1) if not yet done */
    if (cache_size == 0) {
        ensure_cache(1);
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    while (cache_size < n) {
        idx_t m = cache_size + 1;          /* next index to compute */
        seq_t count = 0;                   /* C(m) */

        seq_t mseq = (seq_t)m;
        for (idx_t i = 0; i < cache_size; ++i)
            if (gcd(cache[i], mseq) == (seq_t)1)
                ++count;

        /* Find the smallest multiple of count not already in cache */
        seq_t step = count;                /* count ≥ 1 for m ≥ 2 */
        seq_t cand = step;
        while (contains(cand, cache_size))
            cand += step;

        /* Store the new term */
        ensure_cache(cache_size + 1);
        cache[cache_size++] = cand;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for n = 0; return 0 */
        return 0;
    compute_up_to(n);
    return cache[n - 1];
}
