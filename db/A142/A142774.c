/*
 * A142774  Primes congruent to 47 mod 59.
 * https://oeis.org/A142774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 47 mod 59
 * Range:     u8 n <= 1, u16 n <= 116, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of primes satisfying p ≡ 47 (mod 59) */
    static seq_t *cache = NULL;
    static size_t len   = 0;   /* number of primes cached */
    static size_t cap   = 0;   /* capacity of the cache */
    static idx_t  k      = 0;  /* next k to test, so candidate = 59*k + 47 */

    if (n < 1)
        return 0; /* undefined for n = 0, but return 0 to keep type */

    /* Ensure we have at least n primes cached */
    while (len < (size_t)n) {
        /* candidate number */
        idx_t cand = 59 * k + 47;

        if (is_prime(cand)) {
            /* grow cache if needed */
            if (len == cap) {
                size_t newcap = cap ? cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failed – return 0 to indicate error */
                    return 0;
                }
                cache = tmp;
                cap   = newcap;
            }
            cache[len++] = (seq_t)cand;
        }
        ++k;
    }

    return cache[n - 1];
}
