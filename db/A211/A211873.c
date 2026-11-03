/*
 * A211873  Numbers b >= 0 such that 2 b^2 + 3 b + 5 is prime.
 * https://oeis.org/A211873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 876 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th non‑negative integer b (offset 1) satisfying
   2*b*b + 3*b + 5 is prime.  The function caches previously found
   values, so calls to a() are fast in any order.  It uses the
   helper is_prime_small() for primality testing and dynamic
   reallocation for the cache. */

/* Include the helpers that are needed. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"

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

static seq_t *cache   = NULL;   /* stored b values          */
static size_t cache_cap = 0;    /* allocated capacity       */
static size_t cache_len = 0;    /* number of stored terms   */
static idx_t   next_b    = 0;    /* next b to test           */

/* Ensure cache has at least 'new_cap' slots.  Allocates or grows
   the array using seq_realloc, which is safe for arbitrary-precision
   seq_t objects. */
static void grow_cache(size_t new_cap)
{
    if (cache_cap >= new_cap)
        return;

    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp && new_cap > 0)  /* allocation failure */
        abort();

    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)           /* offset 1; behaviour below is irrelevant */
        return 0;

    /* Fill cache until we have at least n terms. */
    while (cache_len < (size_t)n)
    {
        /* Compute 2*next_b^2 + 3*next_b + 5 using seq_t arithmetic. */
        seq_t val = 2 * (seq_t)next_b * (seq_t)next_b
                  + 3 * (seq_t)next_b + 5;

        if (is_prime_seq(val))
        {
            if (cache_len >= cache_cap)
                grow_cache((cache_cap ? cache_cap * 2 : 16));

            cache[cache_len++] = (seq_t)next_b;
        }
        ++next_b;
    }

    return cache[(size_t)(n - 1)];
}
