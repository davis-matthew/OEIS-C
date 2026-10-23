/*
 * A030162  Cubes such that in n and n^(1/3) the parity of digits alternates.
 * https://oeis.org/A030162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   parity_alternating
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Sequence: a(n) is the n-th cube m^3 where the decimal digits of m and of m^3 alternate
 *  parity (even/odd). The sequence begins with m = 0, 1, 2, 3, 5, 6, 7, 9, 18, 23, ...
 *  This implementation generates the integers sequentially and caches the results.
 */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/parity_alternating.h"

/*
 *  a(idx_t n) returns the n-th entry of A030162.
 *
 *  The function keeps a static cache of computed values.  On each call it
 *  extends the cache until it contains at least n values.
 */
seq_t a(idx_t n)
{
    /* Cache of computed sequence values */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* Last integer m that has been examined (0-based).  cache[0] corresponds to m=0. */
    static idx_t last_m = 0;

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 8;                     /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;              /* memory allocation failed */
        cache[0] = 0;                      /* a(1) = 0^3 */
        cache_len = 1;                     /* one term stored */
        last_m = 0;                        /* last examined m */
    }

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Generate terms until cache_len >= n */
    while (cache_len < n) {
        ++last_m;                          /* next integer to test */
        seq_t m = (seq_t)last_m;
        seq_t cube = m * m * m;             /* m^3 */

        if (parity_alternating(m) && parity_alternating(cube)) {
            /* Ensure sufficient capacity */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0;        /* memory allocation failed */
                cache = tmp;
            }
            cache[cache_len++] = cube;        /* store a(n) = m^3 */
        }
    }

    return cache[n - 1];
}
