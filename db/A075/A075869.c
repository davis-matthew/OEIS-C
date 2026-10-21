/*
 * A075869  Numbers k such that 5*k^2 - 9 is a square.
 * https://oeis.org/A075869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - a(n-2); a(1)=3, a(2)=51
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms (1‑indexed) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;     /* largest n computed so far */
    static size_t cache_cap = 0;

    if (n == 0)
        return (seq_t)0;           /* undefined for n=0, return 0 */

    /* Grow cache if necessary */
    if (n > cache_len) {
        size_t new_cap = (size_t)(n + 1);   /* enough for indices 0..n */
        seq_t *new_cache = NULL;

        if (!cache) {
            new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (!new_cache) {
            /* allocation failed – exit silently */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_cap = new_cap;

        /* Compute new terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)3;
            } else if (i == 2) {
                cache[i] = (seq_t)51;
            } else {
                /* recurrence: a(i) = 18*a(i-1) - a(i-2) */
                cache[i] = (seq_t)((seq_t)18 * cache[i - 1] - cache[i - 2]);
            }
        }
        cache_len = n;
    }

    return cache[n];
}
