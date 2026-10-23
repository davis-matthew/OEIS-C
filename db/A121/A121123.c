/*
 * A121123  Unbranched a-4-catapolynonagons (see Brunvoll reference for
 *          precise definition).
 * https://oeis.org/A121123
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 6*a(n-2) - 36*a(n-3), for n>5 Initial values:
 *            a(2)=1, a(3)=3, a(4)=12, a(5)=63
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_capacity.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* values below the offset are undefined */

    static seq_t *cache = NULL;        /* computed sequence values */
    static size_t cache_cap = 0;       /* current allocated capacity */
    static idx_t cache_len = 0;        /* one past the largest computed index */

    /* initialise first few values */
    if (cache_len == 0) {
        size_t init_cap = 6;                     /* indices 0 .. 5 */
        ensure_capacity(init_cap, &cache_cap, (void**)&cache, sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)12;
        cache[5] = (seq_t)63;
        cache_len = 6;                          /* indices 0..5 are ready */
    }

    if (n < cache_len) return cache[n];        /* already computed */

    /* make sure the array is large enough for n */
    ensure_capacity((size_t)n + 1, &cache_cap, (void**)&cache, sizeof(seq_t));

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 5) {                          /* constants, keep them as is */
            if (i == 2) cache[i] = (seq_t)1;
            else if (i == 3) cache[i] = (seq_t)3;
            else if (i == 4) cache[i] = (seq_t)12;
            else if (i == 5) cache[i] = (seq_t)63;
        } else {
            seq_t term = ((seq_t)6) * cache[i-1]
                       + ((seq_t)6) * cache[i-2]
                       - ((seq_t)36) * cache[i-3];
            cache[i] = term;
        }
    }

    cache_len = n + 1;                         /* now indices 0 .. n are ready */
    return cache[n];
}
