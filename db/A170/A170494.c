/*
 * A170494  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 5·4^(n-1) for 1 ≤ n ≤ 45; for n ≥ 46: a(n) =
 *            3⋅∑_{k=1}^{45} a(n-k) – 6·a(n-46)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The cache holds a[0 … size-1].
    All arithmetic is performed with the type seq_t.  */

#include <stdlib.h>      /* malloc, realloc, free */
#include <stdint.h>      /* for uint64_t if needed */

/* static cache variables */
static seq_t *cache      = NULL;   /* array of computed terms          */
static size_t cache_size = 0;      /* number of elements stored       */
static size_t cache_cap  = 0;      /* allocated capacity of the array */

/* Allocate or enlarge the cache to hold at least n+1 elements.
   For missing indices the values are computed lazily. */
static void ensure_cache(idx_t n)
{
    /* Expand array if necessary */
    if (cache_size <= (size_t)n) {
        size_t needed = (size_t)n + 1;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed)
                new_cap <<= 1;
            seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_mem) abort();            /* allocation failure */
            /* Zero‑initialize the new part for safety */
            for (size_t i = cache_cap; i < new_cap; ++i)
                new_mem[i] = (seq_t)0;
            cache = new_mem;
            cache_cap = new_cap;
        }
    }

    /* Fill in missing terms up to n */

    /* 1. a(0) = 1 */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* 2. a(k) = 5·4^(k-1) for 1 ≤ k ≤ 45 */
    if (cache_size <= 45 && n >= 45) {
        size_t till = 45;
        for (idx_t k = cache_size; k <= till; ++k) {
            /* 4^(k-1) = 1 << (2*(k-1));  works for all seq_t */
            seq_t pow4 = ((seq_t)1) << (2 * (idx_t)(k - 1));
            cache[k] = (seq_t)5 * pow4;
        }
        cache_size = 46;   /* indices 0..45 now valid */
    } else if (cache_size <= 45 && n < 45) {
        /* Only need up to n */
        for (idx_t k = cache_size; k <= n; ++k) {
            seq_t pow4 = ((seq_t)1) << (2 * (idx_t)(k - 1));
            cache[k] = (seq_t)5 * pow4;
        }
        cache_size = (size_t)n + 1;
        return;
    }

    /* 3. Recurrence for indices ≥ 46 */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        /* sum of previous 45 terms */
        for (int k = 1; k <= 45; ++k)
            sum += (seq_t)3 * cache[i - k];
        /* subtract 6·a(i-46) */
        sum -= (seq_t)6 * cache[i - 46];
        cache[i] = sum;
    }
    cache_size = (size_t)n + 1;
}

/*  Public interface required by the challenge.  */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
