/*
 * A099584  Exponent of 3 in factorization of prime(n) - 1.
 * https://oeis.org/A099584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = exponent of 3 in the factorisation of prime(n) - 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a sieve precomputed up to a fixed limit that is
 * large enough to contain at least the first 10 000 primes (the 10 000th prime is 104 729).
 * For each call we ensure that the list of primes is built and then we
 * return the 3‑adic valuation of prime(n)–1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define SIEVE_LIMIT 200000        /* sufficient for n ≤ 10 000 */

static bool *is_composite = NULL;   /* sieve flag array */
static uint64_t *prime_list = NULL; /* array of primes */
static size_t prime_count = 0;      /* number of primes stored */

/* Initialise the sieve and prime list; grows the list if needed. */
static void init_primes(idx_t need)
{
    if (prime_count >= (size_t)need) return;  /* already enough */

    if (!is_composite) {
        /* Allocate and run a classic Eratosthenes sieve. */
        is_composite = (bool*)calloc(SIEVE_LIMIT + 1, sizeof(bool));
        if (!is_composite) exit(1); /* out of memory */

        for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
            if (!is_composite[i]) {
                for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }

        /* Collect primes into the list. */
        size_t cap = SIEVE_LIMIT / 10;  /* initial estimate */
        prime_list = (uint64_t*)malloc(cap * sizeof(uint64_t));
        if (!prime_list) exit(1);

        for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
            if (!is_composite[i]) {
                if (prime_count == cap) {
                    cap *= 2;
                    prime_list = (uint64_t*)realloc(prime_list, cap * sizeof(uint64_t));
                    if (!prime_list) exit(1);
                }
                prime_list[prime_count++] = i;
            }
        }
    }
    /* If the sieve is already built, prime_count should be enough for all n up to limit. */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset 1; behaviour below offset is irrelevant */

    init_primes(n);              /* ensure at least n primes are available */

    uint64_t p = prime_list[(size_t)(n - 1)];  /* prime(n) */
    uint64_t x = p - 1;
    seq_t v3 = 0;

    while (x % 3 == 0) {
        x /= 3;
        ++v3;
    }
    return v3;
}
