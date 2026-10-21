/*
 * A231126  Number of (n+1) X (3+1) white-square subarrays of 0..2 arrays with
 *          no element equal to a strict majority of its diagonal and
 *          antidiagonal neighbors, with values 0..2 introduced in row major
 *          order.
 * https://oeis.org/A231126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 8*a(n-2) - 32*a(n-3) - 38*a(n-4) + 40*a(n-5) +
 *            56*a(n-6) - 16*a(n-7) - 16*a(n-8) Initial values: a(1)=6,
 *            a(2)=40, a(3)=308, a(4)=2260, a(5)=16812, a(6)=124644,
 *            a(7)=924900, a(8)=6862052
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Static cache for memoisation */
static seq_t *cache     = NULL;   /* array of a(0) .. a(max)  */
static idx_t cache_size = 0;      /* allocated size (>= cache_size) */
static idx_t cache_max  = 0;      /* highest index computed (>= cache_max) */

/* Ensure cache can hold index n and all values up to n are computed */
static void ensure_computed(idx_t n)
{
    if (n <= cache_max)          /* already computed */
        return;

    /* Allocate or grow cache so that index n exists */
    if (cache == NULL) {
        cache_size = (size_t)n + 1;
        if (cache_size < 16) cache_size = 16;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) abort();
    } else if (cache_size <= (size_t)n) {
        size_t new_size = cache_size;
        while (new_size <= (size_t)n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        /* Zero‑fill new part */
        memset(tmp + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise first eight terms if they have not been set yet */
    if (cache_max < 8) {
        /* constants for a(1) .. a(8) */
        const int seed_vals[8] = {6, 40, 308, 2260, 16812, 124644, 924900, 6862052};
        for (idx_t i = 1; i <= 8; ++i)
            cache[i] = (seq_t)seed_vals[i - 1];
        cache_max = 8;
    }

    /* Compute the recurrence for all missing indices > cache_max up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val = (seq_t)7 * cache[i - 1]
                  + (seq_t)8 * cache[i - 2]
                  - (seq_t)32 * cache[i - 3]
                  - (seq_t)38 * cache[i - 4]
                  + (seq_t)40 * cache[i - 5]
                  + (seq_t)56 * cache[i - 6]
                  - (seq_t)16 * cache[i - 7]
                  - (seq_t)16 * cache[i - 8];
        cache[i] = val;
    }
    cache_max = n;
}

/* Public accessor for the sequence */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    ensure_computed(n);
    return cache[n];
}
