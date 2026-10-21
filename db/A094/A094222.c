/*
 * A094222  a(n+1) = a(n) + (number of distinct prime factors of a(n)) for
 *          n>1; a(1)=1, a(2)=2.
 * https://oeis.org/A094222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2, a(n+1) = a(n) + Ω(a(n)) (number of distinct
 *            prime factors)
 * Range:     u8 n <= 130, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/count_distinct_primes.h"   /* count_distinct_primes(uint64_t) */

static seq_t *cache = NULL;      /* 0‑based cache of a(1), a(2), …            */
static idx_t cache_size = 0;     /* allocated capacity (in entries)           */
static idx_t computed   = 0;     /* highest index (1‑based) that has been filled */

/* Ensure that cache can hold at least n+1 elements (for indices 0..n). */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n + 1)
        return;

    idx_t new_size = n + 1;          /* need indices 0 … n */

    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
    }
    if (cache == NULL)
        abort();                    /* out of memory */

    cache_size = new_size;
}

/* Initialise the first two terms (idempotent). */
static void init_cache(void)
{
    if (computed >= 2)
        return;

    ensure_cache(2);                /* indices 0 and 1 */

    cache[0] = (seq_t)1;            /* a(1) */
    cache[1] = (seq_t)2;            /* a(2) */
    computed = 2;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* Undefined for n < 1 */

    init_cache();

    if (n <= computed)
        return cache[n - 1];

    /* Compute missing terms up to n. */
    ensure_cache(n);
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];                   /* a(i-1) */
        uint64_t prev_u64 = (uint64_t)prev;          /* safe for n <= 10000 */
        int distinct = count_distinct_primes(prev_u64);
        cache[i - 1] = prev + (seq_t)distinct;       /* a(i) */
    }
    computed = n;
    return cache[n - 1];
}
