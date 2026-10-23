/*
 * A027085  a(n) = A027082(n, n+3).
 * https://oeis.org/A027085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A027085:
   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-4) + a(n-5)   (n >= 5)
   Initial values: a(0)=5, a(1)=16, a(2)=39, a(3)=84, a(4)=169
*/

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t   cache_len = 0;
static const seq_t init[5] = {5, 16, 39, 84, 169};

static void extend_cache(idx_t n)
{
    if (cache_len > n)
        return;

    /* Ensure the underlying storage */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i < 5) {
            val = init[(size_t)i];
        } else {
            val = cache[i-1] * (seq_t)3
                - cache[i-2] * (seq_t)2
                - cache[i-4]
                + cache[i-5];
        }

        /* Grow array if necessary */
        if ((size_t)(i + 1) > cache_cap) {
            size_t new_cap = cache_cap * 2;
            while (new_cap < (size_t)(i + 1))
                new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[i] = val;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    extend_cache(n);
    return cache[n];
}
