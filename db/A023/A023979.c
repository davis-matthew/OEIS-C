/*
 * A023979  Sum of exponents in prime-power factorization of multinomial
 *          coefficient M(4n; n,n,n,n).
 * https://oeis.org/A023979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A022559(4*n) - 4*A022559(n) (sum of exponents in the
 *            prime‑power factorisation of M(4n;n,n,n,n))
 * Range:     u8 n <= 222, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ----- static storage for the prime table ----- */
static uint64_t *primes = NULL;          /* array of primes */
static size_t   primes_cnt = 0;          /* number of primes stored */
static uint64_t primes_max_limit = 0;    /* maximum value for which primes have been generated */

/* ----- generate all primes up to limit (inclusive) ----- */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= primes_max_limit) return;          /* already have enough */

    /* free old prime table if present */
    if (primes != NULL) {
        free(primes);
        primes = NULL;
        primes_cnt = 0;
    }

    /* trivial cases */
    if (limit < 2) {
        primes_max_limit = limit;
        return;
    }

    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool *)calloc(sz, sizeof(bool));
    if (!is_prime) return;                         /* out of memory – leave table empty */

    /* initialise */
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i <= limit; ++i) is_prime[i] = true;

    /* Sieve of Eratosthenes */
    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (uint64_t j = p * p; j <= limit; j += p)
                is_prime[j] = false;
        }
    }

    /* count primes and allocate array */
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++count;

    uint64_t *new_primes = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!new_primes) {
        free(is_prime);
        return;                                 /* allocation failure – keep table empty */
    }

    /* fill array */
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_primes[idx++] = i;

    primes = new_primes;
    primes_cnt = count;
    primes_max_limit = limit;
    free(is_prime);
}

/* ----- compute the sequence value ----- */
seq_t a(idx_t n)
{
    uint64_t ln      = (uint64_t)n;          /* convert index to unsigned 64‑bit */
    uint64_t limit4  = 4 * ln;               /* 4*n */

    /* Ensure primes up to 4*n are available */
    ensure_primes_upto(limit4);

    seq_t sum4 = 0;   /* exponent total in (4*n)!      */
    seq_t sumn = 0;   /* exponent total in n!          */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > limit4) break;              /* primes beyond 4n are irrelevant */

        /* exponent of p in (4*n)! */
        uint64_t power = p;
        while (power <= limit4) {
            sum4 += (seq_t)(limit4 / power);
            power *= p;
        }

        /* exponent of p in n!   */
        if (p <= ln) {
            power = p;
            while (power <= ln) {
                sumn += (seq_t)(ln / power);
                power *= p;
            }
        }
    }

    /* a(n) = sum4 - 4*sumn */
    seq_t result = sum4 - (seq_t)(4 * sumn);
    return result;
}
