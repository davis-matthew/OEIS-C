/*
 * A120940  Alternating sum of the Fibonacci numbers multiplied by their
 *          (combinatorial) indices.
 * https://oeis.org/A120940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - a(n-3) - 3*a(n-4) - a(n-5); initial
 *            values: a(0)=0, a(1)=1, a(2)=3, a(3)=6, a(4)=14
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 40, u64 n <= 84, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache of previously computed terms */
static idx_t cache_size = 0;    /* number of elements currently in cache */
static size_t capacity = 0;     /* allocated capacity of the cache */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        capacity = 5;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;              /* out of memory: return 0 (caller can ignore) */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)14;
        cache_size = 5;
    }

    /* if already computed, just return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* grow the cache if necessary */
    if ((idx_t)capacity <= n) {
        while ((idx_t)capacity <= n)
            capacity *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (!tmp) return 0;  /* out of memory: return 0 */
        cache = tmp;
    }

    /* compute all missing terms up to n using the 5‑term recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = cache[i-1] + 3 * cache[i-2];
        sum -= cache[i-3];
        sum -= 3 * cache[i-4];
        sum -= cache[i-5];
        cache[i] = sum;
    }
    cache_size = n + 1;

    return cache[n];
}
