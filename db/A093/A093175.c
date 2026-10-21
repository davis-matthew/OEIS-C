/*
 * A093175  Tetranacci numbers starting with first four squares.
 * https://oeis.org/A093175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=4, a(3)=9, a(4)=16;
   a(n)=a(n-1)+a(n-2)+a(n-3)+a(n-4) for n ≥ 5 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out‑of‑range guard (not used) */

    /* static cache for previously computed terms */
    static seq_t *cache = NULL;           /* array indexed 0..cache_cap-1 */
    static idx_t cache_size = 0;          /* highest index stored (≥4 after init) */
    static size_t cache_cap = 0;          /* allocated size (≥cache_size+1) */

    /* initialise cache with first four terms */
    if (cache == NULL) {
        cache_cap = 5;                    /* indices 0..4, 0 unused */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;              /* unused placeholder */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)16;
        cache_size = 4;
    }

    /* grow the cache if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    if (cache_size < n) {
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* a(i) = a(i-1)+a(i-2)+a(i-3)+a(i-4) */
            cache[i] = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4];
        }
        cache_size = n;
    }

    return cache[n];
}
