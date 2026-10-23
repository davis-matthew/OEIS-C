/*
 * A014957  Positive integers k that divide 16^k - 1.
 * https://oeis.org/A014957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 643, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   mod_pow, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A014957: the n‑th positive integer k such that
   k divides 16^k − 1.  The implementation searches k sequentially,
   testing each candidate with modular exponentiation. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Memoisation buffers and state. */
    static seq_t *cache     = NULL;   /* stored terms */
    static idx_t cache_len  = 0;      /* number of terms stored */
    static idx_t cache_cap  = 0;      /* current capacity of cache */
    static idx_t next_k      = 1;     /* next k to test */

    if (n == 0)          /* the sequence is 1‑based; behaviour for n<1 is irrelevant */
        return 0;

    /* Grow the cache until we have at least n terms. */
    while (cache_len < n)
    {
        seq_t k      = (seq_t)next_k;
        bool ok      = false;

        if (next_k == 1)           /* 1 | 16^1−1 by definition */
            ok = true;
        else
        {
            seq_t r = mod_pow((seq_t)16, next_k, k);
            if (r == 1)
                ok = true;
        }

        if (ok)
        {
            /* Ensure we have space in the cache. */
            if (cache == NULL)
            {
                cache_cap = 256;
                cache     = seq_alloc(cache_cap);
                if (!cache) abort();
            }
            else if (cache_len == cache_cap)
            {
                idx_t new_cap = cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }

            cache[cache_len++] = k;
        }

        ++next_k;
    }

    return cache[n - 1];  /* zero‑based indexing of the cache array */
}
