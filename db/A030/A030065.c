/*
 * A030065  a(4n)=n, a(4n+2)=a(4n)+a(4n+4), a(2n+1)=a(2n)+a(2n+2).
 * https://oeis.org/A030065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-4) - a(n-8) (n≥8), a(0)=0, a(1)=1, a(2)=1, a(3)=2,
 *            a(4)=1, a(5)=4, a(6)=3, a(7)=5
 * Range:     u8 n <= 340, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 94 terms through n = 866 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of initialized entries */

    /* Ensure the cache can hold at least n+1 elements, but at least 8 for
       the known base values. */
    idx_t need = (n > 7) ? n : 7;
    ensure_cache(need, &cache, &cache_cap);

    /* Initialize base values once. */
    if (cache_len < 8) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)4;
        cache[6] = (seq_t)3;
        cache[7] = (seq_t)5;
        cache_len = 8;
    }

    /* Compute any missing terms up to n. */
    if (cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)2 * cache[i - 4] - cache[i - 8];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
