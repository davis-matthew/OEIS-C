/*
 * A173227  Partial sums of A000262.
 * https://oeis.org/A173227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*a(n-1) - (n^2 - n + 1)*a(n-2) + (n-2)*(n-1)*a(n-3)
 *            Initial values: a(0)=1, a(1)=2, a(2)=5
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* static cache of already-computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t cache_last = 2;                   /* indices 0..2 are initialized */

/* Ensure the cache can hold indices up to n */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_size) return;

    size_t new_size = cache_size ? cache_size * 2 : 4;
    while (new_size <= (size_t)n) new_size <<= 1;

    seq_t *new_cache = seq_alloc_array(new_size);
    if (cache) {
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        seq_free_array(cache);
    }
    cache      = new_cache;
    cache_size = new_size;
}

/* Initialise the cache with the first three terms */
static void init_cache(void)
{
    if (cache) return;                       /* already initialised */

    cache_size = 4;                          /* at least indices 0..3 */
    cache      = seq_alloc_array(cache_size);
    cache[0] = 1;
    cache[1] = 2;
    cache[2] = 5;
    cache_last = 2;
}

seq_t a(idx_t n)
{
    init_cache();
    ensure_cache_capacity(n);

    if (n > cache_last) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];

            seq_t res = (seq_t)(2 * i) * t1;
            seq_t sub = (seq_t)((i * i - i + 1)) * t2;
            res -= sub;
            seq_t add = (seq_t)((i - 2) * (i - 1)) * t3;
            res += add;

            cache[i] = res;
        }
        cache_last = n;
    }
    return cache[(size_t)n];
}
