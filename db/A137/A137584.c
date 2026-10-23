/*
 * A137584  a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3), n > 3.
 * https://oeis.org/A137584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3)
   initial values: a(0)=0, a(1)=3, a(2)=6, a(3)=13 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t last_computed = 0;
    static int initialized = 0;

    /* initialise first four terms once */
    if (!initialized) {
        size_t needed = 4;
        if (needed > cache_cap) {
#ifdef __cplusplus
            seq_t *tmp = new seq_t[needed];
#else
            seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
#endif
            cache = tmp;
            cache_cap = needed;
        }
        cache[0] = 0;
        cache[1] = 3;
        cache[2] = 6;
        cache[3] = 13;
        last_computed = 3;
        initialized = 1;
    }

    size_t idx = (size_t)n;
    if (idx <= last_computed)
        return cache[idx];

    if (idx + 1 > cache_cap) {
#ifdef __cplusplus
        seq_t *tmp = new seq_t[idx + 1];
        for (size_t j = 0; j <= last_computed; ++j) tmp[j] = cache[j];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t*)realloc(cache, (idx + 1) * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
#endif
        cache_cap = idx + 1;
    }

    for (size_t i = last_computed + 1; i <= idx; ++i)
        cache[i] = 3 * cache[i-1] - 2 * cache[i-2] + cache[i-3];

    last_computed = idx;
    return cache[idx];
}
