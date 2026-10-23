/*
 * A126397  Number of base 10 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 7*a(n-1) - 11*a(n-2) + a(n-4) for n>4, with initial
 *            values a(0)=1, a(1)=10, a(2)=44, a(3)=200, a(4)=918 (offset 0,
 *            as in the OEIS entry)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Internal cache for previously computed values */
static seq_t *cache = NULL;          /* pointer to array of seq_t */
static size_t cache_capacity = 0;    /* number of allocated elements */
static idx_t cache_max = 4;          /* highest index that has been computed */

/* Ensure that cache has space for at least indices 0..needed */
static void ensure_cache_cap(size_t needed)
{
    if (cache == NULL) {
        /* Initial allocation: at least 5 elements for the base values */
        size_t init_cap = (needed < 5 ? 5 : needed);
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();            /* allocation failure */
        cache_capacity = init_cap;
        /* Set base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)44;
        cache[3] = (seq_t)200;
        cache[4] = (seq_t)918;
        cache_max = 4;
    } else if (needed + 1 > cache_capacity) {
        /* Grow the array: double the requested size */
        size_t new_cap = (needed + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failure */
        cache = tmp;
        cache_capacity = new_cap;
    }
}

/* Compute a(n) for any n >= 0, using the recurrence
   a(n) = 7*a(n-1) - 11*a(n-2) + a(n-4) for n>4. */
seq_t a(idx_t n)
{
    /* Ensure the cache has the required capacity and base values */
    ensure_cache_cap((size_t)n);

    /* If the value has already been computed, return it */
    if (n <= cache_max)
        return cache[n];

    /* Compute values up to index n, iteratively using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t term1 = cache[i - 1] * (seq_t)7;
        seq_t term4 = cache[i - 4];
        seq_t sum   = term1 + term4;
        seq_t term2 = cache[i - 2] * (seq_t)11;
        cache[i] = sum - term2;        /* safe: result is non‑negative */
    }
    cache_max = n;
    return cache[n];
}
