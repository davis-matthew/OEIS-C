/*
 * A016265  Expansion of 1/((1-x)*(1-10x)*(1-11x)).
 * https://oeis.org/A016265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) - 110*a(n-2) + 1, a(0)=1, a(1)=22
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes coefficients of 1/((1-x)(1-10x)(1-11x)) using the recurrence. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Initialize cache with first two terms. */
static void cache_init(void)
{
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)22;
        cache_size = 2;
    }
}

/* Ensure that cache contains terms 0..n. */
static void ensure_cache(idx_t n)
{
    cache_init();

    if (n < cache_size)
        return;

    size_t new_cap = 2 * (size_t)cache_size;
    if (new_cap <= (size_t)n)
        new_cap = (size_t)n + 1;

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term = cache[i - 1] * (seq_t)21
                   - cache[i - 2] * (seq_t)110
                   + (seq_t)1;
        cache[i] = term;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
