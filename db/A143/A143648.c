/*
 * A143648  a(n) = ((4 + sqrt 6)^n + (4 - sqrt 6)^n)/2.
 * https://oeis.org/A143648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 10*a(n-2); a(0)=1, a(1)=4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void grow_cache(idx_t new_size)
{
    if (new_size > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)new_size);
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    /* initialise base cases once */
    if (cache_size == 0) {
        const idx_t init_size = 2;          /* a(0) and a(1) */
        grow_cache(init_size);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
    }

    /* extend cache up to n, computing missing values */
    if (n >= cache_size) {
        idx_t old_size = cache_size;        /* first index to compute */
        grow_cache((idx_t)n + 1);           /* enlarge storage */
        for (idx_t i = old_size; i <= n; ++i) {
            cache[i] = (seq_t)8 * cache[i-1] - (seq_t)10 * cache[i-2];
        }
    }

    return cache[n];
}
