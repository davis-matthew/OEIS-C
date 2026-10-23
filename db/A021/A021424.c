/*
 * A021424  Expansion of 1/((1-x)(1-3x)(1-5x)(1-7x)).
 * https://oeis.org/A021424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - 86*a(n-2) + 176*a(n-3) - 105*a(n-4) with
 *            a(0)=1, a(1)=16, a(2)=170, a(3)=1520. The recurrence follows
 *            from the generating function 1/((1-x)(1-3x)(1-5x)(1-7x))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache of already computed terms */
static size_t cache_len = 0;    /* number of terms stored */

seq_t a(idx_t n)
{
    /* Grow the cache so that it contains terms up to index n. */
    if ((size_t)n + 1 > cache_len) {
        size_t old_len = cache_len;
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed – return zero */

        /* Initialise newly added slots (they were zeroed by realloc). */
        /* Base cases – only written once, overriding any old garbage. */
        if (new_len > 0)  cache[0] = 1ULL;
        if (new_len > 1)  cache[1] = 16ULL;
        if (new_len > 2)  cache[2] = 170ULL;
        if (new_len > 3)  cache[3] = 1520ULL;

        /* Compute remaining terms using the recurrence. */
        idx_t k;
        for (k = (old_len > 4 ? old_len : 4); k < (idx_t)new_len; ++k)
        {
            cache[k] = 16 * cache[k-1]
                     - 86 * cache[k-2]
                     + 176 * cache[k-3]
                     - 105 * cache[k-4];
        }
        cache_len = new_len;
    }

    return cache[n];
}
