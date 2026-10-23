/*
 * A191651  Ratio A191650(n+1)/A191650(n).
 * https://oeis.org/A191651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) + a(n-5) + a(n-6) - a(n-9)
 *            (empirical), with initial terms: a(1)=2, a(2)=2, a(3)=2, a(4)=4,
 *            a(5)=6, a(6)=9, a(7)=15, a(8)=20, a(9)=28
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* cache[0] unused, cache[i] holds a(i) */
static idx_t   computed_upto = 0;   /* highest n already computed */
static size_t  cache_capacity = 0;  /* allocated size of cache (in terms) */

seq_t a(idx_t n)
{
    /* Return 0 for n==0 (out of bounds, ignored by caller) */
    if (n == 0) return 0;

    /* If we haven't initialized the cache yet, do so up to 9 */
    if (cache == NULL) {
        /* Allocate minimum necessary capacity */
        cache_capacity = (size_t)9 + 1;   /* indices 0..9 */
        cache = (seq_t*)calloc(cache_capacity, sizeof(seq_t));
        if (!cache) return 0;            /* out of memory */
        static const seq_t base[9] = {2,2,2,4,6,9,15,20,28};
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = base[i-1];
        computed_upto = 9;
    }

    /* Grow cache if needed */
    if ((size_t)n + 1 > cache_capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;        /* out of memory */
        /* Zero any new slots (optional) */
        if (new_cap > cache_capacity)
            for (size_t i = cache_capacity; i < new_cap; ++i)
                new_cache[i] = 0;
        cache = new_cache;
        cache_capacity = new_cap;
    }

    /* Compute missing terms up to n */
    if (n > computed_upto) {
        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            seq_t val;
            if (i <= 9) {
                static const seq_t base[9] = {2,2,2,4,6,9,15,20,28};
                val = base[i-1];
            } else {
                val = cache[i-1];
                val += cache[i-3];
                val -= cache[i-4];
                val += cache[i-5];
                val += cache[i-6];
                val -= cache[i-9];
            }
            cache[i] = val;
        }
        computed_upto = n;
    }

    return cache[n];
}
