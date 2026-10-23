/*
 * A132357  a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4), with initial values
 *          1,4,14,41.
 * https://oeis.org/A132357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4) with initial values a(0)=1,
 *            a(1)=4, a(2)=14, a(3)=41
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* cache for memoisation */
static seq_t *seq_cache = NULL;
static idx_t cache_size = 0;   /* allocated capacity (in elements) */
static idx_t computed_to = 0;  /* index of the last computed element */

/* ensure the cache has at least (n+1) entries */
static void ensure_capacity(idx_t n)
{
    if (n + 1 > cache_size)
    {
        idx_t new_cap = cache_size ? cache_size << 1 : 64;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE); /* out‑of‑memory */
        seq_cache = tmp;
        cache_size = new_cap;
    }
}

/* compute the sequence up to index n (inclusive) */
static void compute_upto(idx_t n)
{
    if (!seq_cache)
    {
        /* initial allocation and filling of the first 4 terms */
        cache_size = 64;
        seq_cache = (seq_t*)calloc(cache_size, sizeof(seq_t));
        if (!seq_cache) exit(EXIT_FAILURE);
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)4;
        seq_cache[2] = (seq_t)14;
        seq_cache[3] = (seq_t)41;
        computed_to = 3;
    }
    ensure_capacity(n);
    if (n <= computed_to) return;
    for (idx_t i = computed_to + 1; i <= n; ++i)
    {
        seq_t t1 = seq_cache[i-1];
        seq_t t3 = seq_cache[i-3];
        seq_t t4 = seq_cache[i-4];
        /* a(i) = 3*t1 - t3 + 3*t4  written as 3*t1 + 3*t4 - t3 */
        seq_cache[i] = 3 * t1 + 3 * t4 - t3;
    }
    computed_to = n;
}

/* required external function */
seq_t a(idx_t n)
{
    if (n <= 3)
    {
        static const seq_t init[4] = {
            (seq_t)1,
            (seq_t)4,
            (seq_t)14,
            (seq_t)41
        };
        return init[n];
    }
    compute_upto(n);
    return seq_cache[n];
}
