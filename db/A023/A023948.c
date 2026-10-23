/*
 * A023948  Expansion of 1/((1-x)(1-5x)(1-11x)(1-12x)).
 * https://oeis.org/A023948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*a(n-1) - 275*a(n-2) + 907*a(n-3) - 660*a(n-4) * with
 *            a(0)=1, a(1)=29, a(2)=566, a(3)=9346
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t computed = 0;
    size_t r = (size_t)n;

    /* Ensure the cache is large enough */
    if (r >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap <= r) new_cap = r + 1;
        while (new_cap <= r) new_cap <<= 1;
        if (cache == NULL) {
            cache = seq_alloc(new_cap);
            cache_cap = new_cap;
        } else {
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* Compute missing terms up to index r */
    if (r >= computed) {
        /* Initialize the first four terms if necessary */
        if (computed == 0) {
            cache[0] = (seq_t)1;
            computed = 1;
        }
        if (computed <= 1 && r >= 1) {
            cache[1] = (seq_t)29;
            computed = 2;
        }
        if (computed <= 2 && r >= 2) {
            cache[2] = (seq_t)566;
            computed = 3;
        }
        if (computed <= 3 && r >= 3) {
            cache[3] = (seq_t)9346;
            computed = 4;
        }

        /* Fill the remaining terms using the recurrence */
        for (size_t i = computed; i <= r; ++i) {
            seq_t t1 = (seq_t)29 * cache[i - 1];
            seq_t t2 = (seq_t)275 * cache[i - 2];
            seq_t t3 = (seq_t)907 * cache[i - 3];
            seq_t t4 = (seq_t)660 * cache[i - 4];
            cache[i] = t1 - t2 + t3 - t4;
        }
        computed = r + 1;
    }

    return cache[r];
}
