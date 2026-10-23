/*
 * A043236  Numbers k such that 2 and 6 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   contains26_or62
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes OEIS A043236: numbers k such that digits 2 and 6 are
 * adjacent in the decimal representation of k (in either order),
 * but not in k-1.  The n-th term (1‑based) is returned.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains26_or62.h"

seq_t a(idx_t n) {
    static seq_t *cache      = NULL; /* cached values */
    static idx_t cache_len  = 0;     /* number of cached terms */
    static idx_t cache_cap  = 0;     /* capacity of cache array */

    while (cache_len < n) {
        seq_t last = (cache_len == 0) ? 0 : cache[cache_len - 1];
        idx_t cur  = (idx_t)last + 1;   /* candidate integer */

        for (;;) {
            seq_t k   = (seq_t)cur;
            seq_t km1 = (seq_t)(cur - 1);

            if (contains26_or62(k) && !contains26_or62(km1)) {
                /* store k as next term */
                if (cache_len == cache_cap) {
                    cache_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = k;
                break;
            }
            cur++;
        }
    }
    return cache[n - 1];
}
