/*
 * A038959  Primes p such that 74 is a square mod p.
 * https://oeis.org/A038959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   legendre_74
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 74 is a quadratic residue modulo p
   (a(1) = 2, offset = 1) */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/legendre_74.h"

/* Resize helper for dynamic arrays of uint64_t. */
static void ensure_capacity_uint64(uint64_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : *cap << 1;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    *arr = tmp;
    *cap = new_cap;
}

/* Global storage for primes and the special sequence. */
static uint64_t *prime_list = NULL;
static size_t prime_len = 0, prime_cap = 0;
static uint64_t *good_list = NULL;
static size_t good_len = 0, good_cap = 0;
static uint64_t candidate = 3;
static bool initialized = false;

/* Add a prime to the list. */
static void add_prime(uint64_t p)
{
    ensure_capacity_uint64(&prime_list, &prime_cap, prime_len + 1);
    prime_list[prime_len++] = p;
}

/* Add a good prime to the list. */
static void add_good(uint64_t p)
{
    ensure_capacity_uint64(&good_list, &good_cap, good_len + 1);
    good_list[good_len++] = p;
}

/* Trial‑division primality test using the primes we have found. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t q = prime_list[i];
        if (q * q > n) break;
        if (n % q == 0) return false;
    }
    return true;
}

/* Generate good primes until we have at least 'target' of them. */
static void generate_good_primes(size_t target)
{
    if (!initialized) {
        /* Handle p = 2 separately. */
        add_prime(2);
        add_good(2);          /* 2 divides 74 → a square. */
        initialized = true;
    }

    while (good_len < target) {
        /* Find next prime candidate. */
        while (!is_prime_uint64(candidate)) candidate += 2;

        uint64_t p = candidate;
        add_prime(p);

        /* p == 37 satisfies the condition because 74 ≡ 0 (mod 37). */
        if (p == 37 || legendre_74(p)) {
            add_good(p);
        }

        candidate += 2;   /* next odd candidate */
    }
}

/* Sequence access function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */
    generate_good_primes((size_t)n);
    return (seq_t)good_list[n - 1];
}
