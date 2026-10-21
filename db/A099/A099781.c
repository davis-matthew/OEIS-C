/*
 * A099781  a(n) = Sum_{k=0..floor(n/3)} C(n-k,2*k) * 4^(n-3*k).
 * https://oeis.org/A099781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 16*a(n-2) + a(n-3) Initial values: a(0)=1,
 *            a(1)=4, a(2)=16 (Computed by recurrence for n ≥ 3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_last = (idx_t)-1;

    /* initialize cache with known values up to 2 */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)16;
        cache_last = 2;
    }

    /* grow cache if needed */
    if ((idx_t)n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= (idx_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if ((idx_t)n > cache_last) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            seq_t term;

            if (i == 0) { term = (seq_t)1; }
            else if (i == 1) { term = (seq_t)4; }
            else if (i == 2) { term = (seq_t)16; }
            else {
                /* term = 8*a[i-1] - 16*a[i-2] + a[i-3] */
                term = cache[i-1];
                term *= (seq_t)8;
                term += cache[i-3];
                seq_t sub = cache[i-2];
                sub *= (seq_t)16;
                term -= sub;
            }
            cache[i] = term;
        }
        cache_last = (idx_t)n;
    }

    return cache[(idx_t)n];
}
