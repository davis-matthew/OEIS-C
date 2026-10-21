/*
 * A062602  Number of ways of writing n = p+c with p prime and c nonprime (1
 *          or a composite number).
 * https://oeis.org/A062602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = count of primes p with 1 <= p <= n-1 such that n-p is
 *            non‑prime (1 or composite)
 * Range:     u8 n <= 1624, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t sieve_limit = 0;          /* highest number that has been sieved */
static uint8_t *is_prime = NULL;       /* 1 == prime, 0 == composite or <2 */
static idx_t *prime_list = NULL;       /* list of primes up to sieve_limit */
static size_t prime_count = 0;         /* number of primes stored */
static size_t prime_cap   = 0;         /* capacity of prime_list */

/* Extend the sieve so that all integers up to 'limit' (inclusive) are known. */
static void extend_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    /* Resize the prime flag array */
    size_t new_size = (size_t)(limit + 1);
    is_prime = (uint8_t *)realloc(is_prime, new_size);
    if (!is_prime) exit(EXIT_FAILURE);

    /* Initialise new entries as prime initially */
    for (idx_t i = sieve_limit + 1; i <= limit; ++i)
        is_prime[i] = 1;

    /* 0 and 1 are not prime */
    if (sieve_limit < 2) {
        is_prime[0] = 0;
        if (limit >= 1) is_prime[1] = 0;
    }

    /* Sieve of Eratosthenes up to 'limit' */
    for (idx_t p = 2; (idx_t)p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = 0;
        }
    }

    /* Append newly found primes to the prime list */
    for (idx_t i = sieve_limit + 1; i <= limit; ++i) {
        if (is_prime[i]) {
            if (prime_count == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 8;
                prime_list = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
                if (!prime_list) exit(EXIT_FAILURE);
            }
            prime_list[prime_count++] = i;
        }
    }

    sieve_limit = limit;
}

/* Helper: true iff x is nonprime (1 or composite) */
static bool is_nonprime(idx_t x)
{
    if (x <= 1) return true;           /* 1 is considered nonprime here */
    return !is_prime[x];
}

/* Main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* out of bounds */

    /* Ensure sieve sufficiently large */
    extend_sieve(n);

    seq_t count = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p > n - 1) break;          /* primes are sorted */
        idx_t c = n - p;
        if (is_nonprime(c)) ++count;    /* p + c = n with p prime, c nonprime */
    }
    return count;
}
