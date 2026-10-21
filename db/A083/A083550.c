/*
 * A083550  Product of 2 consecutive prime differences of two successive terms
 *          of A001223.
 * https://oeis.org/A083550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n+1)-prime(n))*(prime(n+2)-prime(n+1))
 * Range:     u8 n <= 325, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a lazy prime sieve with trial division. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* static storage for prime list */
static uint64_t *prime_list   = NULL;  /* array of primes */
static size_t   prime_count  = 0;      /* number of primes stored */
static size_t   prime_cap    = 0;      /* allocated capacity */

/* Append a prime to the list, expanding storage if necessary. */
static void add_prime(uint64_t p)
{
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) exit(1);
    } else if (prime_count == prime_cap) {
        prime_cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        prime_list = tmp;
    }
    prime_list[prime_count++] = p;
}

/* Test whether a candidate is prime, using trial division by known primes. */
static bool is_prime_candidate(uint64_t cand)
{
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > cand / p) break; /* p*p > cand */
        if (cand % p == 0) return false;
    }
    return true;
}

/* Ensure that prime_list contains at least (n+2) primes (1‑based indices). */
static void ensure_primes_up_to(idx_t n)
{
    idx_t needed = n + 2;                 /* number of primes required */
    if (prime_count >= (size_t)needed) return;

    /* Seed with first prime if list is empty. */
    if (prime_count == 0) {
        add_prime(2);
    }

    uint64_t candidate = prime_list[prime_count - 1] + 1;

    while (prime_count < (size_t)needed) {
        if (is_prime_candidate(candidate)) {
            add_prime(candidate);
        }
        ++candidate;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* outside the documented domain */

    ensure_primes_up_to(n);

    /* indices are zero‑based in prime_list */
    seq_t d1 = (seq_t)(prime_list[(size_t)n] - prime_list[(size_t)(n - 1)]);
    seq_t d2 = (seq_t)(prime_list[(size_t)(n + 1)] - prime_list[(size_t)n]);

    return d1 * d2;
}
