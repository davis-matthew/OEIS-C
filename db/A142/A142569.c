/*
 * A142569  Primes congruent to 39 mod 53.
 * https://oeis.org/A142569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 39 (mod 53)
 * Range:     u8 n <= 1, u16 n <= 93, u32 n <= 94, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes and the next arithmetic candidate. */
    static seq_t *primes = NULL;
    static size_t    cap   = 0;  /* allocated capacity */
    static size_t    len   = 0;  /* number of primes stored */
    static seq_t     cand   = 0; /* next candidate in the progression 39 mod 53 */
    static bool      init   = false;

    if (!init) {                 /* first call: initialise */
        cand = 39;
        init = true;
    }

    /* Extend the cache until we have at least n primes. */
    while (len < (size_t)n) {
        if (is_prime_big(cand)) {
            /* Grow the array if necessary. */
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);      /* allocation failure */
                primes = tmp;
                cap = new_cap;
            }
            primes[len++] = cand;
        }
        cand += 53;                 /* next number in the progression */
    }

    return primes[n - 1];
}
