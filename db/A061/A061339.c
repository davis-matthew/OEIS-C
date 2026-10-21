/*
 * A061339  Minimal number of steps to get from 0 to n by (a) adding 1 or (b)
 *          subtracting 1 or (c) multiplying by 2.
 * https://oeis.org/A061339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2n)=a(n)+1, a(2n+1)=min(a(n)+2,a(n+1)+2) (OEIS
 *            A061339)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoization. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache with two entries. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) { return (seq_t)0; } /* Out‑of‑memory protection */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* Extend cache if needed. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) { return (seq_t)0; } /* Out‑of‑memory protection */
        cache = tmp;

        /* Compute new entries using the recurrence. */
        for (idx_t k = cache_size; k <= n; ++k) {
            if (k == 0 || k == 1) {
                cache[k] = (k == 0) ? (seq_t)0 : (seq_t)1;
                continue;
            }
            if ((k & 1) == 0) {                /* even */
                cache[k] = cache[k >> 1] + (seq_t)1;
            } else {                           /* odd */
                seq_t via_n   = cache[k >> 1] + (seq_t)2;      /* a(n)+2  , n = floor(k/2) */
                seq_t via_np1 = cache[(k >> 1) + 1] + (seq_t)2;/* a(n+1)+2 */
                cache[k] = (via_n < via_np1) ? via_n : via_np1;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
