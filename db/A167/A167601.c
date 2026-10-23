/*
 * A167601  G.f.: 1/(1-3*x-40*x^2-192*x^3).
 * https://oeis.org/A167601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 40*a(n-2) + 192*a(n-3), a(0)=1, a(1)=3,
 *            a(2)=49
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;     /* array of computed terms */
static idx_t cached_len = 0;    /* number of terms stored (max index+1) */
static idx_t alloc_cap = 0;     /* allocated capacity */

seq_t a(idx_t n)
{
    /* initialize base values */
    if (cached_len == 0) {
        alloc_cap = 3;
        cache = seq_alloc_array(alloc_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)49;
        cached_len = 3;          /* terms 0..2 computed */
    }

    /* grow the cache if the requested index exceeds current capacity */
    if (n >= alloc_cap) {
        idx_t new_cap = alloc_cap * 2;
        if (new_cap <= n) new_cap = n + 1;   /* ensure space for n */
        seq_t *new_cache = seq_alloc_array(new_cap);
        /* copy existing computed values */
        for (idx_t i = 0; i < cached_len; ++i) {
            new_cache[i] = cache[i];
        }
        seq_free_array(cache);
        cache = new_cache;
        alloc_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n >= cached_len) {
        for (idx_t i = cached_len; i <= n; ++i) {
            cache[i] = 3 * cache[i-1] + 40 * cache[i-2] + 192 * cache[i-3];
        }
        cached_len = n + 1;
    }

    return cache[n];
}
