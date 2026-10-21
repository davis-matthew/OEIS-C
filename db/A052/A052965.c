/*
 * A052965  Expansion of (1-x)/(1-3x-4x^2+4x^3).
 * https://oeis.org/A052965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=10, a(n)=4*a(n-2)+3*a(n-1)-4*a(n-3) for n≥3
 *            (equivalent to 4*a(n)-4*a(n+1)-3*a(n+2)+a(n+3)=0)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap  = 0;

    /* initialise the cache with the first three terms */
    if (cache == NULL) {
        cache_cap   = 3;
        cache_size  = 3;
        cache = (seq_t*)malloc((size_t)cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)10;
    }

    /* fast return if already known */
    if (n < cache_size) {
        return cache[(size_t)n];
    }

    /* grow the cache if necessary */
    idx_t required = n + 1;
    if (required > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < required) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms iteratively using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t t1 = (seq_t)4 * cache[i - 2];
        seq_t t2 = (seq_t)3 * cache[i - 1];
        seq_t val = t1 + t2;
        val -= (seq_t)4 * cache[i - 3];
        cache[i] = val;
    }
    cache_size = n + 1;
    return cache[(size_t)n];
}
