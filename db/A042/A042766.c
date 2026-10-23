/*
 * A042766  Numerators of continued fraction convergents to sqrt(914).
 * https://oeis.org/A042766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(914).
   Recurrence:  a(0)=30, a(1)=121,
   a(n) = p(n) where p(k) = c_k * p(k-1) + p(k-2)
   and c_k (k ≥ 1) repeats the period [4, 3, 3, 4, 60]. */

#include <stdlib.h>

static const unsigned short period[5] = {4, 3, 3, 4, 60};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;  /* number of valid elements stored */

    /* Ensure capacity for indices up to n */
    if (cache_len <= (size_t)n) {
        size_t needed = (size_t)n + 1;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed) new_cap *= 2;
            seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_arr) exit(EXIT_FAILURE);
            cache = new_arr;
            cache_cap = new_cap;
        }

        /* Compute missing terms */
        for (idx_t k = (idx_t)cache_len; k <= n; ++k) {
            seq_t p_k;
            if (k == 0) {
                p_k = (seq_t)30;
            } else {
                unsigned int a_val = period[(k - 1) % 5];
                seq_t a_k = (seq_t)a_val;
                seq_t p_km1 = cache[k - 1];
                seq_t p_km2 = (k > 1) ? cache[k - 2] : (seq_t)1;
                p_k = a_k * p_km1 + p_km2;
            }
            cache[k] = p_k;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
