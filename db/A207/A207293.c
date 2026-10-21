/*
 * A207293  Primes p whose digit sum s(p) is also prime but whose iterated
 *          digit sum s(s(p)) is not prime.
 * https://oeis.org/A207293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   A207293 – primes p with digit sum s(p) prime but s(s(p)) not
 *            prime
 * Range:     u8 n <= 9, u16 n <= 1623, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, is_prime_small, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the helper function prototypes that we use */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/digit_sum.h"
#include "../utils/sieve_primes.h"

/* Static state to keep the prime sieve between calls */
static idx_t *prime_list = NULL;    /* array of primes */
static size_t prime_count  = 0;     /* number of primes in the array */
static idx_t sieve_limit   = 0;     /* upper limit inclusive of primes in prime_list */

/* Rebuild the sieve so that it contains all primes <= needed_limit */
static void ensure_sieve(idx_t needed_limit) {
    if (sieve_limit >= needed_limit)
        return;                    /* already large enough */
    free(prime_list);              /* free old array if any */
    sieve_primes(needed_limit, &prime_list, &prime_count);
    sieve_limit = needed_limit;
}

/* Return the n‑th element of A207293 (n>=1) */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;            /* offset protection */

    /* Rough estimate of the needed sieve size.  200 members per term
       should be more than enough for the first 10,000 terms. */
    idx_t est_limit = n * (idx_t)200;
    if (est_limit < 1000)
        est_limit = 1000;

    /* Loop until we find the n‑th qualifying prime */
    for (;;) {
        ensure_sieve(est_limit);

        size_t cnt = 0;
        for (size_t i = 0; i < prime_count; ++i) {
            seq_t p  = (seq_t)prime_list[i];   /* current prime */
            seq_t s1 = digit_sum(p);           /* first digit sum */

            if (!is_prime_small((idx_t)s1))
                continue;                     /* s1 not prime → reject */

            seq_t s2 = digit_sum(s1);          /* second digit sum */

            if (is_prime_small((idx_t)s2))
                continue;                     /* s2 is prime → reject */

            ++cnt;
            if (cnt == (size_t)n)               /* found the desired term */
                return p;
        }

        /* Not enough terms yet – double the sieve limit and try again */
        if (est_limit >= (idx_t)1ULL << 62)    /* safety guard against overflow */
            est_limit = (idx_t)1ULL << 62;
        est_limit *= 2;
    }
}
