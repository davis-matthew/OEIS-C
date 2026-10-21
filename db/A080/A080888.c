/*
 * A080888  Number of compositions into Fibonacci numbers (1 counted as two
 *          distinct Fibonacci numbers).
 * https://oeis.org/A080888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   grow_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A080888 – Number of compositions into Fibonacci numbers (1 counted twice)
   Formula:  a(0) = 1
             a(n) = Σ_{f∈{1,1,2,3,5,…,≤n}} a(n-f)   (for n ≥ 1) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_to.h"

/* ---------- Memoisation storage ---------- */
static seq_t *cache  = NULL;       /* cache[n] = a(n)   */
static size_t cache_cap = 0;
static size_t cache_size = 0;      /* number of entries already computed (cache[0..cache_size-1] valid) */

/* ---------- Fibonacci numbers with 1 duplicated ---------- */
static seq_t *fib    = NULL;       /* fib[0] = fib[1] = 1, fib[2] = 2, …   */
static size_t fib_cap = 0;
static size_t fib_count = 0;       /* number of Fibonacci elements currently stored */

/* ---------- Ensure that cache and fib tables are computed up to index n ---------- */
static void ensure_cache(idx_t n)
{
    /* Initialise cache on first use */
    if (cache == NULL) {
        grow_to(&cache, &cache_cap, 1);
        cache[0] = (seq_t)1;  /* a(0) = 1 */
        cache_size = 1;
    }

    /* Initialise Fibonacci list on first use */
    if (fib == NULL) {
        grow_to(&fib, &fib_cap, 2);
        fib[0] = fib[1] = (seq_t)1;
        fib_count = 2;
    }

    /* Expand Fibonacci list until the last element exceeds n */
    while (fib[fib_count - 1] <= (seq_t)n) {
        grow_to(&fib, &fib_cap, fib_count + 1);
        fib[fib_count] = fib[fib_count - 1] + fib[fib_count - 2];
        ++fib_count;
    }

    /* Grow cache array if necessary */
    if ((size_t)n + 1 > cache_cap) {
        grow_to(&cache, &cache_cap, (size_t)n + 1);
    }

    /* Compute a(k) for all k in [cache_size … n] */
    for (idx_t k = cache_size; k <= n; ++k) {
        seq_t sum = 0;
        for (size_t j = 0; j < fib_count && fib[j] <= (seq_t)k; ++j) {
            idx_t idx = k - (idx_t)fib[j];
            sum += cache[idx];
        }
        cache[(size_t)k] = sum;
    }

    cache_size = (size_t)n + 1;
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
