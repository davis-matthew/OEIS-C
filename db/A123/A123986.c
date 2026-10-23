/*
 * A123986  Numbers n for which 4n+1 and 4n+3 are primes.
 * https://oeis.org/A123986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1314, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A123986: nth positive integer n such that 4n+1 and 4n+3 are both prime */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;      /* array of terms */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_len = 0;     /* number of terms stored */

    /* last candidate integer that has been examined */
    static idx_t last_checked = 0;

    /* grow cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        /* next candidate value of n */
        ++last_checked;

        idx_t p1 = 4 * last_checked + 1;
        idx_t p3 = 4 * last_checked + 3;

        /* test the primality condition */
        if (is_prime(p1) && is_prime(p3)) {
            /* ensure capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2U : 64U;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);  /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            /* store the term */
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    /* terms are 1‑based, cache is 0‑based */
    return cache[(size_t)(n - 1)];
}
