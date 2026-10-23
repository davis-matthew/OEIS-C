/*
 * A147813  Primes prime(n) such that (-prime(n) + 2*prime(n+1) -
 *          prime(n+2))/((1 - prime(n) + prime(n+1))^(3/2)) < 0.
 * https://oeis.org/A147813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime_k such that (-prime_k + 2*prime_{k+1} -
 *            prime_{k+2}) / (1 - prime_k + prime_{k+1})^(3/2) < 0
 *            (equivalently 2*prime_{k+1} <= prime_k + prime_{k+2})
 * Range:     u8 n <= 31, u16 n <= 3444, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/sieve_primes.h"   /* provides void sieve_primes(idx_t limit,
                                      idx_t **primes_out, size_t *count_out); */

seq_t a(idx_t n)
{
    /* Static state retained between calls */
    static bool initialized = false;
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;
    static seq_t *terms = NULL;        /* sequence terms (primes satisfying
                                          the inequality) */
    static size_t term_len = 0;        /* number of terms stored */

    /* ------------------------------------------------------------------ */
    /* First initialisation:  generate a large enough set of primes and   */
    /* then scan consecutive triples to collect the required terms.       */
    /* ------------------------------------------------------------------ */
    if (!initialized) {
        /* Choose a limit that comfortably contains > 10 000 qualifying   */
        /* primes.  The density of the condition is very high, so a      */
        /* few million is more than enough.                               */
        const idx_t sieve_limit = 2000000;  /* 2 000 000 */

        sieve_primes(sieve_limit, &primes, &prime_cnt);
        if (!primes || prime_cnt < 3) {
            /* In case the sieve failed (unlikely for 2 M) */
            return (seq_t)0;
        }

        /* Allocate the worst-case array size (all primes could qualify) */
        terms = (seq_t *)malloc(prime_cnt * sizeof(seq_t));
        if (!terms) {
            /* Allocation failure – give up safely */
            free(primes);
            primes = NULL;
            return (seq_t)0;
        }

        /* Scan all consecutive triples of primes */
        for (size_t i = 0; i + 2 < prime_cnt; ++i) {
            idx_t p1 = primes[i];
            idx_t p2 = primes[i + 1];
            idx_t p3 = primes[i + 2];

            /* The inequality reduces to 2*p2 <= p1 + p3 */
            if (p2 * 2 <= p1 + p3) {
                terms[term_len++] = (seq_t)p1;
            }
        }

        /* Shrink the terms buffer to the actual size */
        terms = (seq_t *)realloc(terms, term_len * sizeof(seq_t));

        initialized = true;
    }

    /* ------------------------------------------------------------------ */
    /* Return the requested term.  The OEIS offset is 1, so a(1) is the   */
    /* first element stored in terms[].  If n is out of range we return 0. */
    /* ------------------------------------------------------------------ */
    if (n == 0 || n > (idx_t)term_len) {
        return (seq_t)0;
    }
    return terms[n - 1];
}
