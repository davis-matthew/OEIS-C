/*
 * A014913  a(1)=1, a(n) = 24*a(n-1) + n.
 * https://oeis.org/A014913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) + n (a(1) = 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External function signature as requested */
seq_t a(idx_t n)
{
    /* static cache of already computed terms, 1‑based */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size (excluding index 0) */
    static idx_t max_computed = 0;/* largest n with a value stored */

    /* Allocate initial cache size if necessary */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)realloc(cache, (cache_cap + 1) * sizeof(seq_t));
    }

    /* Resize cache if requested index is beyond current capacity */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = 1;
        } else {
            /* a(i) = 24 * a(i-1) + i   (use seq_t arithmetic) */
            cache[i] = cache[i - 1] * 24 + (seq_t)i;
        }
    }
    max_computed = (n > max_computed) ? n : max_computed;

    return cache[n];
}
