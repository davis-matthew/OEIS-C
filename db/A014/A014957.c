/*
 * A014957  Positive integers k that divide 16^k - 1.
 * https://oeis.org/A014957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 643, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 71 terms through n = 1792 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A014957: the n‑th positive integer k such that
   k divides 16^k − 1.  The implementation searches k sequentially,
   testing each candidate with modular exponentiation. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/powmod.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

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
            seq_t r = powmod((seq_t)16, next_k, k);
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
