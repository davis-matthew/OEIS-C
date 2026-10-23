/*
 * A184796  Primes of the form floor(k*sqrt(3)).
 * https://oeis.org/A184796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the Beatty sequence floor(k*sqrt(3))
 *            (OEIS A184796). This implementation avoids floating point by
 *            computing floor(k*sqrt(3)) as floor_sqrt(3*k^2). The primes are
 *            tested using is_prime_big and cached for subsequent calls
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*------------------------------
   Include needed utilities
  ------------------------------*/
#include <stdlib.h>          /* malloc, realloc, free */
#include <stddef.h>          /* size_t */
#include <stdbool.h>         /* bool, true, false */
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */
#include "../utils/is_prime_big.h" /* is_prime_big(seq_t) */

/* -------------------------------
   Compute the n-th term
   -------------------------------*/
seq_t a(idx_t n)
{
    /* n >= 1 for valid queries; return 0 for n==0 */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of found primes */
    static seq_t  *primes    = NULL;    /* array of primes */
    static size_t primes_len = 0;       /* number of primes stored */
    static size_t primes_cap = 0;       /* allocated capacity */
    static idx_t  k_last    = 0;       /* last k examined */

    /* Initialize cache on first call */
    if (primes == NULL) {
        primes_cap = 16;
        primes     = (seq_t *)malloc(primes_cap * sizeof(seq_t));
        if (!primes) return (seq_t)0;  /* allocation failure */
    }

    /* Generate primes until we have at least n of them */
    while (primes_len < (size_t)n) {
        /* Find next candidate k */
        do {
            ++k_last;                                 /* next k */
            seq_t k  = (seq_t)k_last;
            seq_t k2 = k * k;
            seq_t t  = k2 * (seq_t)3;                 /* 3*k^2 */
            seq_t val = floor_sqrt(t);                /* floor(k*sqrt(3)) */

            if (is_prime_big(val)) {
                /* Grow array if needed */
                if (primes_len == primes_cap) {
                    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(primes,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;        /* realloc failed */
                    primes    = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_len++] = val;            /* store prime */
                break;                                 /* found one */
            }
        } while (true);
    }

    return primes[n - 1];
}
