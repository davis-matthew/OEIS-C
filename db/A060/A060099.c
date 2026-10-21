/*
 * A060099  Expansion of g.f. 1/((1-x^2)^3*(1-x)^4).
 * https://oeis.org/A060099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(0) = 1, a(1) = 4, a(n) = (4*a(n-1) + (n+8)*a(n-2)) / n
 *            for n >= 2
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 165, u64 n <= 6877,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static idx_t  cap   = 0;   /* size of allocated cache */
    static seq_t *cache = NULL;/* memoization cache */

    if (n + 1 > cap) {
        /* Allocate (n+1) entries for indices 0 .. n */
        idx_t new_cap = n + 1;
        seq_t *tmp    = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failed */
        cache = tmp;

        /* Compute missing terms up to n */
        for (idx_t i = cap; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)4;
            } else {
#if defined(__SIZEOF_INT128__)
                /* Use 128‑bit arithmetic to avoid overflow */
                typedef unsigned __int128 u128;
                u128 t1 = (u128)4 * (u128)cache[i - 1];
                u128 t2 = (u128)(i + 8) * (u128)cache[i - 2];
                cache[i] = (seq_t)((t1 + t2) / (u128)i);
#else
                /* Fallback: 64‑bit arithmetic (may overflow for large n) */
                seq_t t1 = (seq_t)4 * cache[i - 1];
                seq_t t2 = (seq_t)(i + 8) * cache[i - 2];
                cache[i] = (seq_t)((t1 + t2) / (seq_t)i);
#endif
            }
        }
        cap = new_cap;
    }
    return cache[n];
}
