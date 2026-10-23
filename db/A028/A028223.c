/*
 * A028223  Expansion of 1/((1-7x)(1-8x)(1-10x)(1-12x)).
 * https://oeis.org/A028223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028223: 1/((1‑7x)(1‑8x)(1‑10x)(1‑12x))
   Recurrence:  a(0)=1, a(1)=37, a(2)=863, a(3)=16241
                a(n)=37*a(n-1)-506*a(n-2)+3032*a(n-3)-6720*a(n-4)  (n≥4)     */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached sequence values  */
static size_t cache_capacity = 0;    /* current allocated capacity     */
static size_t cache_size    = 0;     /* number of values stored (next free) */

/* Ensure that the cache can hold at least `needed` elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_capacity)
        return;
    size_t new_cap = cache_capacity ? cache_capacity << 1 : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Out of memory: abort, sequence cannot be produced.  */
        abort();
    }
    cache = tmp;
    cache_capacity = new_cap;
}

/* Initialize the cache with the first four terms. */
static void init_cache(void)
{
    if (cache_size > 0)
        return;
    ensure_capacity(4);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)37;
    cache[2] = (seq_t)863;
    cache[3] = (seq_t)16241;
    cache_size = 4;
}

seq_t a(idx_t n)
{
    /* Handle small indices directly (and initialize for all others). */
    if (n <= 3) {
        init_cache();
        return cache[(size_t)n];
    }

    init_cache();

    /* Compute and cache terms up to `n`. */
    size_t target = (size_t)n;
    if (target >= cache_size) {
        ensure_capacity(target + 1);
        for (size_t i = cache_size; i <= target; ++i) {
            seq_t t1 = cache[i-1] * (seq_t)37;   /* 37*a(i-1)           */
            seq_t t2 = cache[i-2] * (seq_t)506;  /* 506*a(i-2)          */
            seq_t t3 = cache[i-3] * (seq_t)3032; /* 3032*a(i-3)         */
            seq_t t4 = cache[i-4] * (seq_t)6720; /* 6720*a(i-4)         */
            cache[i] = t1 - t2 + t3 - t4;
        }
        cache_size = target + 1;
    }

    return cache[target];
}
