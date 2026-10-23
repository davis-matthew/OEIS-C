/*
 * A164610  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=13, a(2)=156, a(3)=1872, a(4)=22464, a(5)=269568,
 *            a(6)=3234816, a(7)=38817714 a(n) = 11*
 *            (a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)) - 66 * a(n-7) for n
 *            >= 8
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* base values for n = 0 .. 7 */
static const seq_t base_vals[8] = {
    (seq_t)1,
    (seq_t)13,
    (seq_t)156,
    (seq_t)1872,
    (seq_t)22464,
    (seq_t)269568,
    (seq_t)3234816,
    (seq_t)38817714
};

/* initialise cache with the first 8 terms */
static void init_cache(void)
{
    if (cache != NULL) return;
    cache = (seq_t *)malloc(8 * sizeof(seq_t));
    if (cache == NULL) abort();
    for (size_t i = 0; i < 8; ++i) cache[i] = base_vals[i];
    cache_size = 8;
}

/* extend the cache so that it contains terms up to index n */
static void grow_cache(idx_t n)
{
    if (n + 1 <= cache_size) return;
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache == NULL) abort();
    cache = new_cache;

    /* compute new terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 8) {              /* already initialised */
            cache[i] = base_vals[i];
            continue;
        }
        /* i >= 8 */
        seq_t sum = 0;
        for (idx_t k = 1; k <= 6; ++k) sum += cache[i - k];
        seq_t val = (seq_t)11 * sum;
        val -= (seq_t)66 * cache[i - 7];
        cache[i] = val;
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (cache == NULL) init_cache();
    if (n >= cache_size) grow_cache(n);
    return cache[n];
}
