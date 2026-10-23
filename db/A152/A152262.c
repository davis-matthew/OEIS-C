/*
 * A152262  a(n) = 14*a(n-1) - 43*a(n-2), n > 1; a(0)=1, a(1)=7.
 * https://oeis.org/A152262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 43*a(n-2), a(0)=1, a(1)=7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Simple multiplication by a small unsigned integer.
 * The promotion rules guarantee the intermediate result fits
 * within a 'seq_t' when the input is an unsigned integer type.
 */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    return x * m;
}

/* Static cache for memoisation */
static seq_t *cache      = NULL;   /* dynamic array of terms */
static size_t cache_size = 0;        /* number of terms stored */
static size_t cache_cap  = 0;        /* allocated capacity */

/* Ensure the cache has room for at least 'new_cap' terms */
static void grow_cache(size_t new_cap)
{
    if (cache_cap >= new_cap) return;

    size_t ncap = cache_cap ? cache_cap * 2 : 16;
    while (ncap < new_cap) ncap <<= 1;

    seq_t *tmp = (seq_t *)malloc(ncap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);

    if (cache) {
        memcpy(tmp, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = tmp;
    cache_cap = ncap;
}

/* Initialise cache with a(0)=1 and a(1)=7 */
static void init_cache(void)
{
    if (cache) return;           /* already initialised */

    grow_cache(2);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)7;
    cache_size = 2;
}

/* Compute recurrence up to index 'n' (inclusive) */
static void compute_up_to(idx_t n)
{
    init_cache();
    if (cache_size > (size_t)n) return;

    grow_cache((size_t)n + 1);

    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t v1 = mul_small(cache[i - 1], 14);
        seq_t v2 = mul_small(cache[i - 2], 43);
        cache[i] = v1 - v2;          /* unsigned subtraction wraps correctly */
    }
    cache_size = (size_t)n + 1;
}

/* Public entry point required by the build system */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
