/*
 * A188827  Number of 5 X n binary arrays without the pattern 0 1 diagonally
 *          or antidiagonally.
 * https://oeis.org/A188827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-3) + 4*a(n-4) a(1)=32, a(2)=36,
 *            a(3)=120, a(4)=400, a(5)=1360
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 198 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* values below offset are irrelevant */

    static const seq_t first5[5] = {32, 36, 120, 400, 1360};

    if (n <= 5) return first5[n - 1];

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;              /* allocated size (>= n+1) */

    /* initialise the cache on first call */
    if (!cache) {
        cache_size = 6;                       /* indices 0..5 (cache[0] unused) */
        cache = mem_new(seq_t, cache_size);
        if (!cache) exit(1);                  /* allocation failure */
        cache[0] = (seq_t)0;
        for (idx_t i = 1; i <= 5; ++i)
            cache[i] = first5[i - 1];
    }

    /* extend the cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        if (!tmp) { mem_free(cache); exit(1); }   /* allocation failure */
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = (seq_t)((seq_t)4 * cache[i - 1]
                               - (seq_t)8 * cache[i - 3]
                               + (seq_t)4 * cache[i - 4]);
        }
        cache_size = new_size;
    }

    return cache[n];
}
