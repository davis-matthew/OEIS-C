/*
 * A122995  Expansion of x*(1+4*x)/(1-x-25*x^2).
 * https://oeis.org/A122995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 25*a(n-2), a(1) = 1, a(2) = 5
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms already computed */
    static size_t cache_cap = 0;   /* capacity of cache array */

    if (n == 0)
        return 0;                  /* out‑of‑range value; not used in OEIS */

    /* initialise first two terms on first call */
    if (cache_len < 2) {
        ensure_capacity(2, &cache_cap, (void **)&cache, sizeof(seq_t));
        cache[0] = 1;              /* a(1) */
        cache_len = 1;
        if (cache_len < 2) {
            cache[1] = 5;          /* a(2) */
            cache_len = 2;
        }
    }

    /* grow cache until it contains a(n) */
    while (cache_len < (size_t)n) {
        seq_t next = cache[cache_len - 1] + (seq_t)25 * cache[cache_len - 2];
        ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
        cache[cache_len] = next;
        ++cache_len;
    }

    return cache[n - 1];
}
