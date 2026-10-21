/*
 * A087128  a(1)=1 and, for n>1, a(n) is the smallest positive integer such
 *          that 1+Sum_{k=a(n-1)+1..a(n)} k is prime.
 * https://oeis.org/A087128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A087128: a(1)=1; for n>1, a(n) is the smallest t>a(n‑1) such that
 * 1 + Σ_{k=a(n‑1)+1}^{t} k is prime.  The routine computes terms
 * incrementally, memoising already‑found values.  It uses an incremental
 * sum to avoid repeated recomputation of the arithmetic progression.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrime.h"          /* bool isPrime(idx_t) */

/* -----  cache for previously computed terms  ----- */
static seq_t *cache   = NULL;   /* 0‑based: cache[i] = a(i+1) */
static idx_t  cache_len = 0;    /* number of stored terms   */
static idx_t  cache_cap = 0;    /* allocated capacity       */

/* enlarge the cache so that it can hold at least `needed` terms */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* compute the next term after `prev_a`.  `prev_a` is a(n‑1). */
static seq_t compute_next(seq_t prev_a)
{
    seq_t sum = 0;
    seq_t t   = prev_a + 1;           /* start searching at a(n‑1)+1 */

    for (;; ++t) {
        sum += t;
        seq_t candidate = sum + 1;     /* 1 + Σ_{k=prev_a+1}^{t} k */
        if (isPrime((idx_t)candidate))
            return t;
    }
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for offset 1 */

    /* initialise cache with a(1) if empty */
    if (cache_len == 0) {
        grow_cache(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* grow cache to target size if needed */
    if (cache_len < n)
        grow_cache(n);

    /* fill missing entries */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = compute_next(prev);
    }
    cache_len = n;

    return cache[n - 1];
}
