/*
 * A105907  Primes for which -35 is a primitive root.
 * https://oeis.org/A105907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime p (n≥1) for which –35 is a primitive *
 *            root modulo p. The routine builds a sieve, tests each prime, and
 *            * returns the requested term
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mod_pow.h"   /* uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t m) */

/* State: a dynamically growing sieve and list of primes. */
static bool *sieve = NULL;
static uint32_t sieve_limit = 0;              /* current size of the sieve */

static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_capacity = 0;

/* Primes for which –35 is a primitive root. */
static seq_t *accepted = NULL;
static size_t acc_count = 0;
static size_t acc_capacity = 0;

/* Test whether –35 is a primitive root modulo a prime p. */
static bool is_primitive_root(uint32_t p)
{
    if (p == 2) return true;            /* 1 is the unique unit mod 2 */
    if (p == 5 || p == 7) return false; /* –35 ≡ 0 (mod p) */

    const uint32_t rem = 35 % p;
    const uint32_t base = (rem == 0) ? 0 : p - rem;
    if (base == 0) return false;        /* divisible by p */

    const uint32_t phi = p - 1;
    uint32_t temp = phi;

    for (size_t i = 0; i < prime_count; ++i) {
        const uint32_t q = prime_list[i];
        if ((uint64_t)q * (uint64_t)q > temp) break;
        if (temp % q == 0) {
            const uint64_t exp = (uint64_t)phi / q;
            if (mod_pow((uint64_t)base, exp, (uint64_t)p) == 1) return false;
            while (temp % q == 0) temp /= q;
        }
    }
    if (temp > 1) {
        const uint64_t exp = (uint64_t)phi / temp;
        if (mod_pow((uint64_t)base, exp, (uint64_t)p) == 1) return false;
    }
    return true;
}

/* Rebuild the sieve, prime list, and accepted list up to new_limit. */
static void extend_sieve(uint32_t new_limit)
{
    if (new_limit <= sieve_limit) return;

    /* Allocate a new sieve and run Eratosthenes. */
    bool *new_sieve = (bool*)calloc((size_t)new_limit + 1, sizeof(bool));
    if (!new_sieve) exit(1);
    new_sieve[0] = true;
    if (new_limit >= 1) new_sieve[1] = true;

    for (uint32_t p = 2; (uint64_t)p * p <= new_limit; ++p)
        if (!new_sieve[p])
            for (uint32_t j = p * p; j <= new_limit; j += p)
                new_sieve[j] = true;

    free(sieve);
    sieve = new_sieve;
    sieve_limit = new_limit;

    /* Build the list of primes up to new_limit. */
    free(prime_list);
    prime_count = 0;
    prime_capacity = 0;
    for (uint32_t i = 2; i <= new_limit; ++i) {
        if (!sieve[i]) {
            if (prime_count >= prime_capacity) {
                prime_capacity = prime_capacity ? prime_capacity * 2 : 16;
                uint32_t *tmp = (uint32_t*)realloc(prime_list, prime_capacity * sizeof(uint32_t));
                if (!tmp) exit(1);
                prime_list = tmp;
            }
            prime_list[prime_count++] = i;
        }
    }

    /* Build the list of accepted primes. */
    free(accepted);
    acc_count = 0;
    acc_capacity = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        const uint32_t p = prime_list[i];
        if (is_primitive_root(p)) {
            if (acc_count >= acc_capacity) {
                acc_capacity = acc_capacity ? acc_capacity * 2 : 16;
                seq_t *tmp2 = (seq_t*)realloc(accepted, acc_capacity * sizeof(seq_t));
                if (!tmp2) exit(1);
                accepted = tmp2;
            }
            accepted[acc_count++] = (seq_t)p;
        }
    }
}

/* Return the n-th term of A105907 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n=0 – avoid UB */

    /* Ensure we have at least n accepted primes. */
    while ((idx_t)acc_count < n) {
        uint32_t new_limit = (sieve_limit == 0) ? 100000U : sieve_limit * 2U;
        extend_sieve(new_limit);
    }
    return accepted[(size_t)(n - 1)];
}
