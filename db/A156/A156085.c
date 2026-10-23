/*
 * A156085  One fourth of the sum of the squares of the first n Fibonacci
 *          numbers with index divisible by 3.
 * https://oeis.org/A156085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1)+17*a(n-2)-a(n-3) a(0)=0, a(1)=1, a(2)=17
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    idx_t new_len = n + 1;
    seq_t *new_cache = NULL;

    if (cache == NULL) {
        new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache)
            exit(1);
        cache = new_cache;
    } else if (new_len > cache_len) {
        new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)
            exit(1);
        cache = new_cache;
    }

    /* Initialise the first three terms if they are not yet set */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
    }
    if (cache_len <= 1 && new_len > 1) {
        cache[1] = (seq_t)1;
    }
    if (cache_len <= 2 && new_len > 2) {
        cache[2] = (seq_t)17;
    }

    /* Compute further terms using the recurrence */
    idx_t start = (cache_len > 3) ? cache_len : 3;
    if (start < 3)
        start = 3;
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = ( (seq_t)17 * cache[i-1] )
                 + ( (seq_t)17 * cache[i-2] )
                 - cache[i-3];
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
