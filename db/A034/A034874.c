/*
 * A034874  a(1) = 1; for n >= 2, a(n) = n times the reverse of a(n-1).
 * https://oeis.org/A034874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, nice
 * Formula:   a(1) = 1; for n ≥ 2, a(n) = n * reverse_decimal(a(n‑1))
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 33,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n) {
    static seq_t *cache    = NULL;   /* 1‑based: cache[i‑1] = a(i) */
    static idx_t  cache_cap = 0;     /* allocated capacity */
    static idx_t  cache_size = 0;    /* number of computed terms */

    if (n == 0) return (seq_t)0;      /* safety, not used */

    /* Ensure enough capacity */
    if (n > cache_cap) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed */ return (seq_t)0; }
        /* Zero the newly allocated part */
        if (new_cap > cache_cap) {
            memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        }
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n that have not yet been computed */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)1;
        } else {
            seq_t rev = reverse_decimal(cache[i - 2]);
            cache[i - 1] = (seq_t)i * rev;
        }
    }
    cache_size = n;

    return cache[n - 1];
}
