/*
 * A060308  Largest prime <= 2n.
 * https://oeis.org/A060308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest prime ≤ 2·n
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (By definition of OEIS sequence A060308) */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* static state for the prime sieve and prime list */
static idx_t sieve_limit = 0;          /* maximum value already sieved (inclusive) */
static uint8_t *sieve = NULL;          /* sieve[i] == 0 ⇒ i is prime, 1 ⇒ composite */
static idx_t *prime_list = NULL;       /* array of primes found, in ascending order */
static size_t prime_cnt = 0;           /* number of primes stored */
static size_t prime_cap = 0;           /* capacity of prime_list */

/* --------------------------------------------------------------------- */
/* Rebuild the Eratosthenes sieve up to at least 'limit' (inclusive).   */
/* The algorithm is O(limit log log limit) and sufficient for n up to 10 000. */
/* --------------------------------------------------------------------- */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;                   /* already up to date */

    /* Free old structures */
    free(sieve);
    free(prime_list);
    sieve = NULL;
    prime_list = NULL;
    prime_cnt = 0;
    prime_cap = 0;

    /* Allocate new sieve buffer */
    sieve = (uint8_t *)malloc((size_t)(limit + 1));
    if (!sieve)
        exit(EXIT_FAILURE);
    memset(sieve, 0, (size_t)(limit + 1));

    /* Simple Eratosthenes sieve. */
    for (idx_t i = 2; i <= limit; ++i) {
        if (sieve[i] == 0) {          /* i is prime */
            /* Grow prime list if necessary */
            if (prime_cnt == prime_cap) {
                if (prime_cap == 0)
                    prime_cap = 16;
                else
                    prime_cap <<= 1;
                prime_list = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
                if (!prime_list)
                    exit(EXIT_FAILURE);
            }
            prime_list[prime_cnt++] = i;

            /* Mark multiples of i */
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    sieve[j] = 1;
            }
        }
    }

    sieve_limit = limit;
}

/* --------------------------------------------------------------------- */
/* Return the largest prime ≤ x (x ≥ 2).                                */
/* --------------------------------------------------------------------- */
static seq_t previous_prime(idx_t x)
{
    while (x >= 2 && sieve[x])
        --x;
    return (seq_t)x;
}

/* --------------------------------------------------------------------- */
/* Public sequence function as required by the build system.             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)           /* undefined for n = 0; return 0 gracefully */
        return 0;

    idx_t limit = 2 * n;
    ensure_sieve(limit);
    return previous_prime(limit);
}
