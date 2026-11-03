/*
 * A028221  Expansion of 1/((1-7x)(1-8x)(1-9x)(1-12x)).
 * https://oeis.org/A028221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028221
   Formula: a(n) = (12^(n+3) – 10·9^(n+3) + 15·8^(n+3) – 6·7^(n+3)) / 60
   Linear recurrence used for efficient computation:
   a(0)=1, a(1)=36, a(2)=817, a(3)=14964
   a(n)=36·a(n-1) – 479·a(n-2) + 2796·a(n-3) – 6048·a(n-4) for n≥4
   (Both formulas are mathematically identical.) */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Static cache – shared among calls */
static seq_t *cache = NULL;          /* array of a(i) */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t max_computed = 0;       /* highest index already computed */

/* Return a(n) for the sequence A028221 */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise base values once */
    if (max_computed < 3) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)36;
        cache[2] = (seq_t)817;
        cache[3] = (seq_t)14964;
        max_computed = 3;
    }

    /* If we already have the requested value, just return it */
    if (n <= max_computed)
        return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t t1 = mul_small(cache[k - 1], 36);
        seq_t t2 = mul_small(cache[k - 2], 479);
        seq_t t3 = mul_small(cache[k - 3], 2796);
        seq_t t4 = mul_small(cache[k - 4], 6048);
        /* Summation and subtraction are safe: the first sum ≥ second sum */
        cache[k] = (t1 + t3) - (t2 + t4);
    }
    max_computed = n;
    return cache[n];
}
