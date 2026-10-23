/*
 * A013739  a(n) = 6^(3*n + 2).
 * https://oeis.org/A013739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(3*n + 2) = 36*216^n Recurrence: a(0) = 36, a(n) =
 *            216*a(n-1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements currently filled */
    static idx_t cache_cap = 0;   /* allocated capacity in cache */

    /* Ensure enough space for indices 0..n */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory – aborting is safe */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing entries using the recurrence */
    if (n + 1 > cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)
                cache[i] = (seq_t)36;
            else
                cache[i] = (seq_t)216 * cache[i - 1];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
