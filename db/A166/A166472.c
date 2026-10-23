/*
 * A166472  a(n) = 2^F(n+2)*3^F(n+1)/12, where F(n) is the n-th Fibonacci
 *          number (A000045(n)).
 * https://oeis.org/A166472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^F(n+2)*3^F(n+1)/12 Recurrence: a(1)=1, a(2)=6,
 *            a(n)=12*a(n-1)*a(n-2) for n>2 where F(k) is the k‑th Fibonacci
 *            number
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"

static seq_t *cache = NULL;        /* array of computed terms, 1‑based */
static idx_t  cache_cap = 0;       /* current allocated size (indices 0..cache_cap-1) */
static idx_t  cache_max = 0;       /* largest index for which cache[i] is valid */

/* Ensure that cache[1..n] are available (and computed) */
static void ensure_cache(idx_t n)
{
    /* Allocate or grow cache array if necessary */
    if (cache_cap <= n) {
        idx_t new_cap = n + 1;                      /* indices 0..n */
        seq_t *new_cache = seq_alloc((size_t)new_cap);
        if (cache) {
            for (idx_t i = 0; i < cache_cap; ++i)
                new_cache[i] = cache[i];
            /* the old array will be leaked, but is harmless for the
               short‑lived nature of a competition program */
        }
        cache = new_cache;
        cache_cap = new_cap;

        /* Initialize base values if not already present */
        if (cache_cap > 1) cache[1] = 1;
        if (cache_cap > 2) cache[2] = 6;
    }

    /* Compute missing terms up to n */
    if (cache_max < n) {
        idx_t start = (cache_max < 2) ? 3 : cache_max + 1;
        for (idx_t i = start; i <= n; ++i) {
            seq_t prod = cache[i - 1];
            prod *= cache[i - 2];
            prod *= 12;
            cache[i] = prod;
        }
        cache_max = n;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1, but this protects callers */

    ensure_cache(n);
    return cache[n];
}
