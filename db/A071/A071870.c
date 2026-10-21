/*
 * A071870  Numbers k such that gpf(k) > gpf(k+1) > gpf(k+2) where gpf(k)
 *          denotes the largest prime factor of k.
 * https://oeis.org/A071870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A071870:  Numbers k such that gpf(k) > gpf(k+1) > gpf(k+2).
   Computed by a simple scan; no direct formula. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gpf.h"

/* Static cache of found terms (values are of type seq_t). */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Next candidate integer k to test (starts at the first known term). */
static uint64_t next_k = 13;   /* the smallest k satisfying the condition */

/* Ensure the cache contains at least 'needed' terms. */
static void ensure_cache(idx_t needed)
{
    while (cache_len < needed) {
        uint64_t k    = next_k;
        uint64_t a    = gpf(k);
        uint64_t b    = gpf(k + 1);
        uint64_t c    = gpf(k + 2);

        if (a > b && b > c) {          /* found a term of the sequence */
            if (cache_len == cache_cap) {   /* grow the cache */
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();               /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
        ++next_k;                        /* advance to next candidate */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset is 1; behaviour for n < 1 is irrelevant */
    ensure_cache(n);
    return cache[n - 1];
}
