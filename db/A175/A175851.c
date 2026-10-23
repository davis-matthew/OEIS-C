/*
 * A175851  a(n) = 1 for noncomposite n, a(n) = n - previousprime(n) + 1 for
 *          composite n.
 * https://oeis.org/A175851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = 1 for noncomposite n, a(n) = n - previousprime(n) + 1 for
 *            composite n
 * Range:     u8 n <= 9451, u16 n <= 9451, u32 n <= 9451, u64 n <= 9451,
 *            u128 n <= 9451, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


/* ---------- static state for the sieve ---------- */
static idx_t   sieve_limit   = 0;          /* largest n for which the sieve is built   */
static uint8_t *sieve        = NULL;       /* 1 = prime, 0 = composite, index 0..limit */
static idx_t   *prime_list   = NULL;       /* list of primes up to sieve_limit         */
static size_t  prime_count   = 0;          /* number of stored primes                   */

/* Build an Eratosthenes sieve up to 'limit' (inclusive) and the prime list. */
static void build_sieve(idx_t limit)
{
    if (limit == sieve_limit) return;     /* already built up to this limit */

    /* allocate and initialise sieve array to all 1s (true) */
    sieve = (uint8_t*)calloc(limit + 1, sizeof(uint8_t));
    if (!sieve) abort();                  /* out of memory */
    for (idx_t i = 0; i <= limit; ++i) sieve[i] = 1;
    sieve[0] = 0;
    sieve[1] = 0;

    /* Eratosthenes marking */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (sieve[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                sieve[m] = 0;
        }
    }

    /* Allocate prime list (roughly limit/log(limit) primes). A generous upper bound. */
    size_t est_primes = limit / 2 + 10;
    prime_list = (idx_t*)calloc(est_primes, sizeof(idx_t));
    if (!prime_list) abort();

    /* Populate list */
    size_t pc = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (sieve[i]) {
            if (pc == est_primes) {
                /* enlarge array if needed */
                est_primes *= 2;
                prime_list = (idx_t*)realloc(prime_list, est_primes * sizeof(idx_t));
                if (!prime_list) abort();
            }
            prime_list[pc++] = i;
        }
    }
    prime_count = pc;
    sieve_limit = limit;
}

/* Ensure the sieve contains all primes up to at least n. */
static void ensure_sieve(idx_t n)
{
    if (n > sieve_limit) build_sieve(n);
}

/* Return the greatest prime ≤ n. For n < 2 it returns 1 as a sentinel. */
static idx_t previous_prime(idx_t n)
{
    if (n < 2) return 1;            /* treat 1 as "previous prime" for n = 1 */
    for (idx_t x = n; x >= 2; --x) {
        if (sieve[x]) return x;
    }
    return 1;                      /* fallback (should never happen for n≥2) */
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n <= 1) return 1;           /* a(1) = 1 */

    ensure_sieve(n);                /* make sure we can test primality of n */

    if (sieve[n]) return 1;         /* n is a prime or 1 → value 1 */

    /* composite case: n - previous_prime(n) + 1 */
    idx_t prev = previous_prime(n - 1);   /* previous prime < n */
    return (seq_t)n - (seq_t)prev + 1;
}
