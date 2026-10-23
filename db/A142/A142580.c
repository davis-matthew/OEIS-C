/*
 * A142580  Primes congruent to 50 mod 53.
 * https://oeis.org/A142580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 109, u32 n <= 111, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A142580: primes congruent to 50 (mod 53) – found by incremental search */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of known primes ≡ 50 (mod 53) */
    static seq_t *primes   = NULL;
    static size_t   cap    = 0;
    static size_t   cnt    = 0;
    static uint64_t cand   = 103;   /* first such prime */

    /* safety for out‑of‑range n, not used by tests */
    if (n == 0)
        return 0;

    /* initialise cache once */
    if (primes == NULL) {
        cap = 16;
        primes = (seq_t *)malloc(cap * sizeof(seq_t));
        if (primes == NULL)
            return 0;   /* allocation failure – caller may ignore */
    }

    /* generate more primes until we reach the requested index */
    while (cnt < (size_t)n) {
        /* grow the array if needed */
        if (cnt == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
            if (tmp == NULL)
                return 0;   /* allocation failure */
            primes = tmp;
            cap = new_cap;
        }

        /* test the current candidate for primality */
        if (is_prime_big((seq_t)cand))
            primes[cnt++] = (seq_t)cand;

        /* advance to next number ≡ 50 (mod 53) */
        cand += 53;
    }

    /* return the (1‑based) requested prime */
    return primes[(size_t)n - 1];
}
