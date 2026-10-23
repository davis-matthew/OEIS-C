/*
 * A123090  a(1)=1. For n>=2, a(n) = a(n-1) + (largest divisor of n which
 *          occurs among the earlier terms of the sequence).
 * https://oeis.org/A123090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>=2, * a(n)=a(n-1)+max{ d | d divides n and d
 *            occurs among a(1) … a(n-1) }
 * Range:     u8 n <= 32, u16 n <= 1060, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   value_in_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps the sequence values in a sorted array,
 * so a divisor can be tested for membership by binary search. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/value_in_cache.h"

/* static storage for the sequence */
static seq_t *cache       = NULL;   /* 1‑based array of a(n) */
static idx_t  computed   = 0;       /* highest index computed */
static size_t cache_size = 0;       /* current capacity of cache */

/* enlarge the cache array if necessary */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { /* allocation failed */
            free(cache);
            cache      = NULL;
            cache_size = 0;
            computed   = 0;
            return;
        }
        cache = tmp;
        cache_size = new_size;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but safe */

    if (n > computed) {
        /* extend cache */
        ensure_cache_capacity(n);

        /* compute terms from (computed+1) up to n */
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 1) {               /* initial value */
                cache[1] = (seq_t)1;
                continue;
            }

            seq_t best = (seq_t)1;      /* 1 is always a divisor of n */

            /* enumerate all divisors of i */
            for (idx_t d = 1; d <= i / d; ++d) {   /* avoid overflow in d*d */
                if (i % d == 0) {
                    seq_t val = (seq_t)d;
                    if (value_in_cache(cache, i - 1, val) && val > best)
                        best = val;

                    idx_t e = i / d;
                    if (e != d) {             /* the paired divisor */
                        val = (seq_t)e;
                        if (value_in_cache(cache, i - 1, val) && val > best)
                            best = val;
                    }
                }
            }

            cache[i] = cache[i - 1] + best;
        }
        computed = n;
    }

    return cache[n];
}
