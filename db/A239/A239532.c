/*
 * A239532  Number of (n+1) X (3+1) 0..2 arrays with no element equal to all
 *          horizontal neighbors or unequal to all vertical neighbors, and new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A239532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A239532: Number of (n+1)×(3+1) arrays over {0,1,2} with
 * the stated constraints, new values introduced in row‑major order.
 *
 * The data satisfy the linear recurrence
 *
 *   a(n) = 8*a(n-1) + 26*a(n-2) - 166*a(n-3)
 *          + 96*a(n-4) + 198*a(n-5) - 81*a(n-6)
 *
 * with the initial values
 *   a(1)=6, a(2)=6, a(3)=216, a(4)=1014,
 *   a(5)=13254, a(6)=98304.
 *
 * The implementation keeps a cache so that repeated calls
 * are O(1): the sequence is computed once up to the
 * largest n requested.
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset; the sequence is defined for n>=1. */
    if (n < 1) {
        return 0;
    }

    /* Static cache for memoisation */
    static seq_t *cache = NULL;        /* array indexed from 1 */
    static idx_t  cache_size = 0;      /* allocated size (including dummy 0) */
    static idx_t  last_computed = 0;   /* highest index in cache that is valid */

    /* Ensure the cache is large enough */
    if (cache == NULL) {
        /* Allocate just enough for the first few terms + some slack */
        cache_size = 64;
        cache = (seq_t *)calloc((size_t)cache_size, sizeof(seq_t));
        if (!cache) abort();
    } else if ((idx_t)cache_size <= n) {
        /* Grow the cache to twice the required size */
        size_t new_size = (size_t)(n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        /* initialise the newly allocated part to zero */
        size_t old_size = (size_t)cache_size;
        for (size_t i = old_size; i < new_size; ++i) tmp[i] = 0;
        cache = tmp;
        cache_size = (idx_t)new_size;
    }

    /* Initialise the first six terms if not already done */
    if (last_computed < 6) {
        cache[1] = (seq_t) 6;
        cache[2] = (seq_t) 6;
        cache[3] = (seq_t)216;
        cache[4] = (seq_t)1014;
        cache[5] = (seq_t)13254;
        cache[6] = (seq_t)98304;
        last_computed = 6;
    }

    /* If the requested term is already in the cache, return it */
    if (n <= last_computed) {
        return cache[n];
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t v = cache[i-1];
        v = v * (seq_t)8 + cache[i-2] * (seq_t)26;
        v = v + cache[i-4] * (seq_t)96 + cache[i-5] * (seq_t)198;
        v = v - cache[i-3] * (seq_t)166 - cache[i-6] * (seq_t)81;
        cache[i] = v;
    }
    last_computed = n;
    return cache[n];
}
