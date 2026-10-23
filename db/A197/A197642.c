/*
 * A197642  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 4,2,2,0,2
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + a(n-3) + 14*a(n-4) - 9*a(n-5) -
 *            8*a(n-6). Initial values: a(1)=1, a(2)=2, a(3)=11, a(4)=31,
 *            a(5)=66, a(6)=185
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0; /* allocated capacity */
static idx_t cache_size = 0; /* number of terms already computed */

static const seq_t init_vals[7] = {0, 1, 2, 11, 31, 66, 185};

/* Ensure the cache contains the first 6 terms. */
static void init_cache(void)
{
    if (cache == NULL) {
        cache_cap = 6;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        for (idx_t i = 0; i < 6; ++i) {
            cache[i] = init_vals[i + 1];
        }
        cache_size = 6;
    }
}

/* Grow the cache capacity to at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = (cache_cap == 0) ? 6 : cache_cap * 2;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute terms up to count 'n' (1‑based) if not already cached. */
static void compute_up_to(idx_t n)
{
    for (idx_t i = cache_size; i < n; ++i) {
        /* i ≥ 6 because cache_size is at least 6. */
        seq_t val = cache[i - 1];
        val = val * 2 + cache[i - 2] + cache[i - 3] + cache[i - 4] * 14
              - cache[i - 5] * 9 - cache[i - 6] * 8;
        cache[i] = val;
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* safety for offset 1 */
    init_cache();
    grow_cache((size_t)n);
    if (cache_size < n)
        compute_up_to(n);
    return cache[n - 1];
}
