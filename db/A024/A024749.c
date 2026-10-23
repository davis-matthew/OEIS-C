/*
 * A024749  Binomial coefficients: C(n,k), 5 <= k <= n-5, sorted.
 * https://oeis.org/A024749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th smallest binomial coefficient C(n,k) with 5 ≤ k
 *            ≤ n-5, sorted in non‑decreasing order. This function generates
 *            all such coefficients for n ≥ 10, keeps them in a dynamically
 *            resizing sorted array, and returns the n-th entry
 * Range:     u8 none, u16 n <= 17, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

/* Compare two seq_t values for qsort. */
static int cmp_seq_t(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    return (*a < *b) ? -1 : (*a > *b) ? 1 : 0;
}

/* Dynamic cache of sorted binomial values. */
static seq_t *cache      = NULL;   /* array of values */
static size_t cache_cnt  = 0;      /* number of stored values */
static size_t cache_cap  = 0;      /* allocated capacity   */
static idx_t  last_n_done = 9;     /* last n processed (n<10 yields no values) */

/* Ensure the cache can hold at least `needed` entries. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache      = tmp;
    cache_cap  = new_cap;
}

/* Return the n-th term of A024749 (offset 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* behaviour below the offset is irrelevant */

    /* Extend the cache until the n-th term is stable. */
    for (;;)
    {
        /* If we already have at least n values, check if new values
           could affect the n-th term. */
        if (cache_cnt >= (size_t)n)
        {
            seq_t nth_val = cache[n - 1];
            idx_t next_n = last_n_done + 1;
            if (next_n >= 10)
            {
                seq_t next_min = binomial(next_n, 5); /* smallest value for newer n */
                if (next_min >= nth_val)
                    break;   /* future values are all ≥ nth_val, no effect */
            }
            else
                break;       /* no further values will be generated */
        }

        /* Generate the next block of coefficients for n = last_n_done + 1. */
        ++last_n_done;
        if (last_n_done >= 10)
        {
            for (idx_t k = 5; k <= last_n_done - 5; ++k)
            {
                grow_cache(cache_cnt + 1);
                cache[cache_cnt++] = binomial(last_n_done, k);
            }
            /* Keep the cache sorted after adding the new block. */
            qsort(cache, cache_cnt, sizeof(seq_t), cmp_seq_t);
        }
    }

    return cache[n - 1];   /* zero‑based indexing */
}
