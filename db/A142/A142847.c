/*
 * A142847  Primes congruent to 49 mod 61.
 * https://oeis.org/A142847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime congruent to 49 modulo 61
 * Range:     u8 none, u16 n <= 102, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Necessary utilities  */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                /* out of offset */

    /*  Static cache of found primes  */
    static seq_t *primes = NULL;               /* array of primes */
    static size_t  cap   = 0;                  /* allocated capacity   */
    static size_t  len   = 0;                  /* number of primes stored */
    static seq_t   cand  = (seq_t)49;           /* current candidate   */

    /*  Allocate cache when first used  */
    if (cap == 0) {
        cap = 16;
        primes = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!primes) return (seq_t)0;          /* allocation failed */
    }

    /*  Generate primes until we have at least n of them  */
    while (len < (size_t)n) {
        if (is_prime_big(cand)) {              /* candidate is prime ? */
            /*  Grow the array if necessary  */
            if (len == cap) {
                size_t new_cap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;    /* allocation failed */
                primes = tmp;
                cap = new_cap;
            }
            primes[len++] = cand;              /* store prime          */
        }
        cand = cand + (seq_t)61;                /* next number ≡49 (mod 61) */
    }

    return primes[n - 1];                      /* return the nth prime */
}
