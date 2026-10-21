/*
 * A238705  Number of partitions of 4n into 4 parts with smallest part = 1.
 * https://oeis.org/A238705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5)
   a(1)=1, a(2)=4, a(3)=10, a(4)=19, a(5)=30 */

#include <stddef.h>
#include <stdlib.h>

/* External integer types, defined by the build */
extern seq_t a(idx_t n);

static seq_t *cache = NULL;          /* 1‑based storage, index 0 unused */
static idx_t cache_size = 0;         /* current capacity */

/* Helper: enlarge cache if needed (capacity >= n+1) */
static void ensure_capacity(idx_t n)
{
    if (n < cache_size) return;
    idx_t new_size = n + 1;             /* include index 0 */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
    if (!new_cache) exit(1);            /* out of memory */
    cache = new_cache;
    for (idx_t i = cache_size; i < new_size; ++i)
        cache[i] = (seq_t)0;
    cache_size = new_size;
}

/* Base values */
static const seq_t base_vals[6] = {0, 1, 4, 10, 19, 30};

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1, but return 0 */

    /* Return pre‑computed if n <=5 */
    if (n <= 5) return base_vals[n];

    /* Ensure cache capacity and initialize base if first use */
    if (cache_size == 0) {
        ensure_capacity(5);
        for (idx_t i = 1; i <= 5; ++i)
            cache[i] = base_vals[i];
    } else if (cache[1] == 0) {      /* first call after allocation */
        for (idx_t i = 1; i <= 5; ++i)
            cache[i] = base_vals[i];
    }

    /* Expand cache up to n */
    ensure_capacity(n);

    /* Compute values up to n */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t val = 2 * cache[i-1];
        val = val - cache[i-2];
        val = val + cache[i-3];
        /* subtract 2*cache[i-4] without temporary negative */
        val = val - (cache[i-4] + cache[i-4]);
        val = val + cache[i-5];
        cache[i] = val;
    }

    return cache[n];
}
