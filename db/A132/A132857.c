/*
 * A132857  a(0)=1. a(n) = phi(n+a(n-1)), for n>=1, where phi(m) is the number
 *          of positive integers which are <= m and are coprime to m.
 * https://oeis.org/A132857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = phi(n + a(n-1)), for n >= 1
 * Range:     u8 n <= 115, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;    /* number of elements currently stored */

    /* Extend the cache if it does not yet hold a(n) */
    if ((idx_t)cache_len <= n) {
        size_t new_len = (size_t)n + 1;
        /* realloc/malloc return void*, cast to seq_t* for C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp && new_len) {
            abort();  /* allocation failed – terminate safely */
        }
        cache = tmp;

        /* Compute any new entries sequentially */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else {
                /* n + a(n‑1) – cast idx_t to seq_t before adding */
                seq_t sum = (seq_t)i + cache[i - 1];
                cache[i] = calc_phi(sum);
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
