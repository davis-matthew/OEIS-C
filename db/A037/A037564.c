/*
 * A037564  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,2,1.
 * https://oeis.org/A037564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{k=0}^{n-1} d_k · 7^{n-1-k} where d_k = 2 if k % 3 < 2
 *            else 1 (period 2,2,1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined below offset */

    /* cache[1..] holds computed values; index 0 is unused but allocated */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* biggest n computed so far */
    static size_t cache_cap = 0;   /* capacity of cache array */

    if (n > cache_size) {
        /* ensure capacity for indices 0..n */
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < (size_t)n + 1)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort();            /* out of memory */
        cache = new_cache;
        cache_cap = new_cap;

        if (cache_size == 0) {
            cache[1] = (seq_t)2;   /* a(1) = 2 */
            cache_size = 1;
        }

        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t digit = ((i - 1) % 3 < 2) ? (seq_t)2 : (seq_t)1;
            cache[i] = cache[i - 1] * (seq_t)7 + digit;
        }
        cache_size = n;
    }
    return cache[n];
}
