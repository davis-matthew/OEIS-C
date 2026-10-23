/*
 * A103729  Column k=2 sequence of array A103728.
 * https://oeis.org/A103729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5 - 4*p(n+2) + p(n+2)^2) / 2 where p(k) is the k‑th
 *            prime
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/estimate_prime_upper_bound.h"

static uint8_t *sieve_composite = NULL;   /* flag array for composites */
static uint64_t sieve_limit = 0;          /* largest number sieved so far */

static uint64_t *prime_table = NULL;      /* stored primes */
static idx_t prime_cap = 0;               /* allocated size of prime_table */
static idx_t prime_count = 0;             /* number of primes found */

/* Ensure all primes up to 'limit' are available in prime_table */
static void ensure_primes(uint64_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Allocate/combine sieve array */
    free(sieve_composite);
    sieve_composite = (uint8_t *)malloc((size_t)(limit + 1));
    if (!sieve_composite)
        abort();
    memset(sieve_composite, 0, limit + 1);

    /* Reset prime list */
    prime_count = 0;
    if (prime_cap < (size_t)(limit / 10 + 100)) {
        free(prime_table);
        prime_cap = limit / 10 + 100;
        prime_table = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_table)
            abort();
    }

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!sieve_composite[i]) {              /* i is prime */
            if (prime_count == prime_cap) {
                prime_cap *= 2;
                prime_table = (uint64_t *)realloc(
                    prime_table, prime_cap * sizeof(uint64_t));
                if (!prime_table)
                    abort();
            }
            prime_table[prime_count++] = i;
            if (i <= limit / i) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    sieve_composite[j] = 1;
            }
        }
    }
    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    /* m = n+2 is the index of the prime used */
    idx_t m = n + 2;
    uint64_t bound = estimate_prime_upper_bound(m);
    ensure_primes(bound);

    /* If we accidentally didn't accumulate enough primes, extend the sieve */
    if (prime_count < m) {
        uint64_t new_bound = bound * 2;
        ensure_primes(new_bound);
        if (prime_count < m) {          /* still not enough? try again */
            bound = estimate_prime_upper_bound(m);
            ensure_primes(bound);
        }
    }

    uint64_t p = prime_table[m - 1];     /* the m‑th prime (p(n+2)) */
    seq_t sp   = (seq_t)p;               /* promote to seq_t */
    seq_t val  = ((seq_t)5 + sp * sp - (seq_t)4 * sp) / ((seq_t)2);
    return val;
}
