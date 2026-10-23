/*
 * A130910  Partial sums of A130909.
 * https://oeis.org/A130910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-16) - a(n-17)
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=0, a(1)=1, a(2)=3, a(3)=6, a(4)=10, a(5)=15, a(6)=21, a(7)=28, */
/* a(8)=36, a(9)=45, a(10)=55, a(11)=66, a(12)=78, a(13)=91, a(14)=105, a(15)=120. */
/* For n < 0 the terms are treated as 0. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* memoisation array */
static idx_t  cache_size = 0;        /* number of initialised entries */
static size_t cache_cap  = 0;        /* allocated capacity (in seq_t elements) */

/* Ensure that entries up to index n (inclusive) are computed and stored. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Initial capacity can be arbitrary; 128 gives low fragmentation. */
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;  /* out of memory; caller will get garbage */

        /* Known first 16 values (indices 0..15). */
        const seq_t init_vals[16] = {
            0, 1, 3, 6, 10, 15, 21, 28,
            36, 45, 55, 66, 78, 91, 105, 120
        };
        for (idx_t i = 0; i < 16; ++i) cache[i] = init_vals[i];
        cache_size = 16;
    }

    if (n < cache_size) return;   /* already computed */

    /* Extend capacity if necessary. */
    while (cache_size <= n) {
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return;  /* out of memory; computation stops early */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute a(cache_size) using the recurrence. */
        idx_t i = cache_size;   /* new index to compute */
        seq_t val = cache[i - 1];
        if (i >= 16) val += cache[i - 16];
        if (i >= 17) val -= cache[i - 17];
        cache[i] = val;

        ++cache_size;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n < cache_size) return cache[n];
    /* In case of allocation failure or other error, return 0. */
    return 0;
}
