/*
 * A041315  Denominators of continued fraction convergents to sqrt(171).
 * https://oeis.org/A041315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 340*a(n-2) - a(n-4) Initial values: a(0)=1, a(1)=13,
 *            a(2)=339, a(3)=4420
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Expand the cache if needed */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – aborting is reasonable for this utility */
            abort();
        }
        cache = new_cache;

        /* Compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == (idx_t)0)
                cache[0] = (seq_t)1;
            else if (i == (idx_t)1)
                cache[1] = (seq_t)13;
            else if (i == (idx_t)2)
                cache[2] = (seq_t)339;
            else if (i == (idx_t)3)
                cache[3] = (seq_t)4420;
            else
                cache[i] = ((seq_t)340 * cache[i - 2]) - cache[i - 4];
        }
        cache_size = new_size;
    }
    return cache[n];
}
