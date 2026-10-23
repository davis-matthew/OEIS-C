/*
 * A024900  Numbers k such that 7*k + 6 is prime.
 * https://oeis.org/A024900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 838, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find k such that 7*k + 6 is prime; a(n) returns the n‑th such k (n≥1) */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Undefined for n = 0 – return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    /* Cached sequence */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of cached terms */
    static size_t cache_cap = 0;  /* allocated capacity */
    static idx_t next_k = 1;      /* next candidate k */

    while (cache_len < n) {
        /* Compute 7*k + 6 */
        seq_t val = (seq_t)7 * (seq_t)next_k + 6;

        /* Test primality */
        if (is_prime_big(val)) {
            /* Ensure capacity for cache */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();  /* unrecoverable memory error */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
