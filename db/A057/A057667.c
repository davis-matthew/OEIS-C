/*
 * A057667  Primes q of form q = 10p + 3, where p is an odd prime.
 * https://oeis.org/A057667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime_uint64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes q such that q = 10*p + 3 where p is an odd prime.
   The function builds the sequence on demand, caching previously
   found terms.  It uses next_prime() to enumerate odd primes p and
   is_prime_uint64() to test primality of both p and q. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, return 0 */

    static seq_t *cache = NULL;  /* cached results */
    static size_t cap   = 0;     /* cache capacity */
    static idx_t  used  = 0;     /* number of cached terms */
    static uint64_t next_p = 3;  /* next odd prime to test (starts at 3) */
    static bool initialized = false;

    /* initialise on first call */
    if (!initialized) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        used  = 0;
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (used < n) {
        /* current odd prime candidate */
        uint64_t p = next_p;

        /* advance to next odd prime for future iterations */
        next_p = next_prime(next_p);

        /* q = 10*p + 3 */
        uint64_t q = 10 * p + 3;

        if (is_prime_uint64(p) && is_prime_uint64(q)) {
            /* grow the cache if necessary */
            if (used == cap) {
                size_t new_cap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failure; abort */
                    free(cache);
                    return 0;
                }
                cache = tmp;
                cap   = new_cap;
            }
            cache[used++] = (seq_t)q;
        }
    }

    return cache[n - 1];
}
