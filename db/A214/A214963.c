/*
 * A214963  a(0)=a(1)=1, a(n) = least k > a(n-1) such that k*a(n-2) is an
 *          oblong number.
 * https://oeis.org/A214963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > a(n-1) such that k * a(n-2) is an oblong number
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 31, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialisation */
    if (cache_len == 0) {
        cache_cap = (n > 1) ? n + 1 : 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cache_len = 2;
    }

    /* expand cache if necessary */
    if (n >= cache_len) {
        size_t new_cap = cache_cap;
        while (n >= new_cap) {
            new_cap = (new_cap == 0) ? 2 : new_cap * 2;
        }
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 2) {
            cache[i] = 1;                /* a(0)=a(1)=1 */
        } else {
            seq_t prevprev = cache[i - 2];
            seq_t prev     = cache[i - 1];
            seq_t k = prev + 1;          /* start checking from a(n-1)+1 */

            for (;;) {
                seq_t prod = k * prevprev;
                seq_t d    = prod << 2;   /* 4 * prod */
                d += 1;                  /* 1 + 4*prod */

                seq_t s = floor_sqrt(d); /* largest integer ≤ sqrt(d) */
                if (s * s == d && (s & 1)) {
                    cache[i] = k;
                    break;
                }
                ++k;
            }
        }
    }
    cache_len = n + 1;
    return cache[n];
}
