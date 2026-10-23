/*
 * A045443  Primes congruent to {0, 1, 5, 6} mod 7.
 * https://oeis.org/A045443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 0, 1, 5, or 6 modulo 7
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Computes the n‑th term of OEIS A045443.
   It caches all found terms and only runs a simple primality test
   on consecutive integers.  The cache is expanded lazily so that
   multiple calls (in any order) return correct results. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n < 1 */

    /* static cache variables */
    static seq_t *cache = NULL;      /* array of cached terms */
    static size_t   cap   = 0;       /* current capacity of cache */
    static idx_t   len   = 0;        /* number of terms cached */
    static idx_t   cur   = 2;        /* current integer to test */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 32;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort(); /* allocation failure */
    }

    /* grow cache while we need more terms */
    while (len < n) {
        /* only odd numbers >2 are needed, but scanning all is fine */
        if (is_prime(cur)) {
            idx_t m = cur % 7;
            if (m == 0 || m == 1 || m == 5 || m == 6) {
                /* ensure capacity */
                if (len + 1 >= cap) {
                    cap <<= 1;
                    seq_t *tmp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                }
                cache[len++] = (seq_t)cur;
            }
        }
        cur++;
    }

    return cache[n - 1];
}
