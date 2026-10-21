/*
 * A065890  Number of composites less than the n-th prime.
 * https://oeis.org/A065890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = prime(n) - n - 1
 * where prime(n) is the n-th prime (1‑based).
 * The number of composites < prime(n) is thus
 *   prime(n) - (n + 1).
 *
 * The implementation generates primes on demand using simple trial division
 * and stores them in a static vector to handle arbitrary index order.
 * No underflow can occur because the n‑th prime is always ≥ n+1.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* idx_t is defined by the build environment (e.g., uint64_t). */
static uint64_t *prime_list = NULL;   /* dynamic array of computed primes */
static size_t prime_count = 0;        /* number of primes stored */

/* Grow prime_list to accommodate `new_count` primes. */
static void grow_prime_list(size_t new_count)
{
    if (prime_count >= new_count) return;

    /* Expand capacity so that space grows geometrically. */
    size_t new_cap = prime_count ? prime_count * 2 : 16;
    while (new_cap < new_count) new_cap *= 2;

    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    prime_list = tmp;
    /* prime_count remains unchanged; only capacity increases. */
}

/* Extend the prime list so that it contains at least `n` primes. */
static void ensure_n_primes(idx_t n)
{
    if (prime_count >= (size_t)n) return;           /* already enough */

    grow_prime_list((size_t)n);                     /* allocate more space */

    /* Start searching for primes from 2 if list empty, otherwise from last prime + 1. */
    uint64_t candidate = prime_count ? prime_list[prime_count - 1] + 1 : 2;

    while (prime_count < (size_t)n) {
        bool is_prime = true;
        /* Trial division by all stored primes up to sqrt(candidate). */
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            prime_list[prime_count++] = candidate;
        }
        ++candidate;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* The sequence is 1‑based; return 0 for n == 0 (undefined case). */
    if (n == 0) return 0;

    ensure_n_primes(n);                             /* make sure nth prime is available */
    seq_t prime_n = (seq_t)prime_list[(size_t)n - 1];/* prime(n) */
    seq_t res = prime_n - ((seq_t)n + (seq_t)1);   /* prime(n) - (n + 1) */
    return res;
}
