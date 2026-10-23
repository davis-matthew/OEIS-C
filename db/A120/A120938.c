/*
 * A120938  a(0)=1. a(n) = a(n-1) + (largest integer occurring among
 *          {a(0),a(1),a(2),...,a(n-1)} that is coprime to n).
 * https://oeis.org/A120938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   for n > 0, a(n) = a(n‑1) + M,
   where M is the largest value among {a(0), …, a(n‑1)} that is coprime to n. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *seq_cache = NULL;   /* cache of computed terms */
static size_t cache_len = 0;      /* number of cached entries (next free index) */

/* Ensure that seq_cache contains values 0 … n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len > (size_t)n) return; /* already computed */

    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
    if (!new_cache) abort();            /* allocation failed */
    seq_cache = new_cache;

    /* If this is the first call, initialise a(0) = 1. */
    if (cache_len == 0) {
        seq_cache[0] = 1;
        cache_len = 1;
    }

    /* Compute missing entries sequentially. */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        seq_t max_coprime = 0;

        /* Search backwards from i-1 down to 0 for a coprime term. */
        for (idx_t j = i - 1; ; --j) {
            if (gcd(seq_cache[j], (seq_t)i) == 1) {
                max_coprime = seq_cache[j];
                break;                /* largest found */
            }
            if (j == 0) break;        /* no more candidates */
        }

        /* a(0) = 1 guarantees that max_coprime is set for all n>0. */
        seq_cache[i] = seq_cache[i - 1] + max_coprime;
    }

    cache_len = new_len;   /* update cached length */
}

/* Public entry point required by the evaluation framework. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[(idx_t)n];
}
