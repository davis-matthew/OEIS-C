/*
 * A173919  Numbers that are prime or one less than a prime.
 * https://oeis.org/A173919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: integers k such that k or k+1 is prime (offset 1) */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* provides: bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* Static state */
    static seq_t *cache      = NULL;   /* array of terms */
    static size_t cache_size = 0;      /* number of stored terms */
    static size_t cache_cap  = 0;      /* allocated capacity */
    static idx_t  next_test  = 2;      /* next integer to examine */

    /* n == 0 is outside the defined offset; return 0 (unused) */
    if (n == 0) return 0;

    /* Initialise on first call */
    if (cache_size == 0) {
        cache_cap = 64;                      /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failed */
        cache[0] = (seq_t)1;                 /* a(1) = 1  (1+1 is prime) */
        cache_size = 1;
    }

    /* Grow the cache until it contains at least n terms */
    while (cache_size < n) {
        /* Test whether current integer satisfies the property */
        bool ok = is_prime(next_test) || is_prime(next_test + 1);
        if (ok) {
            /* Grow the cache array if necessary */
            if (cache_size == cache_cap) {
                cache_cap = cache_cap * 2;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0;        /* allocation failed */
                cache = tmp;
            }
            /* Store the qualifying integer */
            cache[cache_size++] = (seq_t)next_test;
        }
        ++next_test;
    }

    /* Return the n‑th term (1‑based index) */
    return cache[n - 1];
}
