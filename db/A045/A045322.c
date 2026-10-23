/*
 * A045322  Primes congruent to {0, 2, 3, 4, 6} (mod 7).
 * https://oeis.org/A045322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A045322: primes congruent to {0,2,3,4,6} modulo 7; obtained by incremental prime search. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"  /* provides bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* memoized filtered primes */
    static size_t cache_len = 0;    /* number of stored primes */
    static size_t cache_cap = 0;    /* allocated capacity of cache */
    static idx_t cur = 2;           /* next integer to test for primality */

    if (n == 0)                 /* offset is 1; behavior for n<1 is irrelevant */
        return 0;

    while (cache_len < n) {
        if (is_prime(cur)) {
            idx_t r = cur % 7;
            if (r == 0 || r == 2 || r == 3 || r == 4 || r == 6) {
                /* grow cache if necessary */
                if (cache_cap == 0) {
                    cache_cap = 8;
                    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
                    if (!cache) abort();
                } else if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap << 1;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)cur;
            }
        }
        cur++;
    }

    return cache[(size_t)(n - 1)];
}
