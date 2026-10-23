/*
 * A102704  Numbers k such that k999 is prime.
 * https://oeis.org/A102704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th positive integer k such that 1000*k + 999 is prime */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n = 0; return 0 as a safeguard */
        return (seq_t)0;
    }

    /* Static cache of already found terms (k values) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of terms stored */
    static idx_t cache_capacity = 0;  /* allocated capacity */
    static idx_t last_k = 0;          /* last k that was examined */

    /* Grow the cache until we have at least n terms */
    while (cache_size < n) {
        idx_t k = last_k + 1;                         /* next candidate k   */
        seq_t candidate = ((seq_t)k) * 1000 + 999;    /* value to test prime */

        if (is_prime_seq(candidate)) {                /* 1000*k+999 is prime? */
            if (cache_size == cache_capacity) {       /* grow capacity if needed */
                cache_capacity = cache_capacity ? cache_capacity << 1 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failed; return 0 as an error code */
                    return (seq_t)0;
                }
                cache = tmp;
            }
            cache[cache_size++] = k;                  /* store k, not the prime value */
        }

        last_k = k;    /* advance k for the next iteration */
    }

    return cache[n - 1];
}
