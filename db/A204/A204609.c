/*
 * A204609  Number of (n+1) X 2 0..1 arrays with the determinants of 2 X 2
 *          subblocks nondecreasing rightwards and downwards.
 * https://oeis.org/A204609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-3) for n>6 with initial values: a(1)=16,
 *            a(2)=47, a(3)=125, a(4)=335, a(5)=907, a(6)=2470
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return trivial values for the first six terms */
    if (n == 0) return (seq_t)0;
    if (n <= 6) {
        switch (n) {
            case 1: return (seq_t)16;
            case 2: return (seq_t)47;
            case 3: return (seq_t)125;
            case 4: return (seq_t)335;
            case 5: return (seq_t)907;
            case 6: return (seq_t)2470;
            default: return (seq_t)0;   /* unreachable */
        }
    }

    /* Static cache of already computed terms */
    static seq_t *cache      = NULL;   /* array of terms a(1)...a(sz) */
    static size_t cache_len  = 0;      /* number of terms stored */
    static size_t cache_cap  = 0;      /* allocated capacity */

    /* One‑time initialization of the cache */
    if (cache_len == 0) {
        /* Enough room for a few terms beyond the seed */
        cache_cap = 16;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure – unlikely */

        /* Seed with the first six values */
        cache[0] = (seq_t)16;   /* a(1) */
        cache[1] = (seq_t)47;   /* a(2) */
        cache[2] = (seq_t)125;  /* a(3) */
        cache[3] = (seq_t)335;  /* a(4) */
        cache[4] = (seq_t)907;  /* a(5) */
        cache[5] = (seq_t)2470; /* a(6) */
        cache_len = 6;
    }

    /* Extend the cache up to the requested index n */
    while (cache_len < (size_t)n) {
        /* Grow the storage if necessary */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) return (seq_t)0; /* allocation failure – unlikely */
        }
        /* Compute next term using the recurrence:
           a(k) = 3*a(k-1) - 2*a(k-3)   for k>6   */
        seq_t val = (seq_t)3 * cache[cache_len - 1] - (seq_t)2 * cache[cache_len - 3];
        cache[cache_len++] = val;
    }

    /* Return the requested term (array is zero‑based, indices 0..cache_len-1) */
    return cache[(size_t)n - 1];
}
