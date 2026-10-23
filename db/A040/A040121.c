/*
 * A040121  Primes p such that x^4 = 10 has no solution mod p.
 * https://oeis.org/A040121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A040121: primes p such that x^4 = 10 has no solution.
    The property holds for a prime p>5 iff 10^((p‑1)/gcd(4,p‑1)) ≠ 1 (mod p),
    where gcd(4,p‑1) = 4 when p ≡ 1 (mod 4) and 2 otherwise.  All primes
    dividing 10 (2,5) are excluded.  The function builds up a cache of
    such primes incrementally and returns the n‑th one for n ≥ 1.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* Cached primes that satisfy the property, stored as 64‑bit values. */
static uint64_t *cached_primes = NULL;
static size_t     cached_len   = 0;
static size_t     cached_cap   = 0;
/* next odd integer to test (starts after 3). */
static uint64_t  next_candidate = 3;

/* Ensure that the cache has room for at least 'need' primes. */
static void ensure_capacity(size_t need)
{
    if (cached_cap >= need) return;
    size_t new_cap = cached_cap ? cached_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    uint64_t *tmp = (uint64_t *)realloc(cached_primes, new_cap * sizeof(uint64_t));
    if (!tmp && new_cap != 0) abort();
    cached_primes = tmp;
    cached_cap    = new_cap;
}

/* Test whether a prime p satisfies the condition and store it if it does. */
static void test_and_store(uint64_t p)
{
    /* Skip primes that divide 10. */
    if (p == 2 || p == 5) return;

    /* gcd(4,p-1) is 4 if p ≡ 1 (mod 4), else 2. */
    uint64_t g = ((p - 1) & 2) == 0 ? 4 : 2;
    uint64_t exp = (p - 1) / g;

    /* Compute 10^exp mod p. */
    seq_t val = mod_pow((seq_t)10, exp, (seq_t)p);
    if (val != (seq_t)1) {
        ensure_capacity(cached_len + 1);
        cached_primes[cached_len++] = p;
    }
}

/* Generate the cache until it contains at least 'need' primes. */
static void generate_until(size_t need)
{
    while (cached_len < need) {
        /* Advance to the next odd candidate. */
        ++next_candidate;
        if ((next_candidate & 1) == 0) continue;  /* Skip even numbers. */

        if (is_prime(next_candidate)) {
            test_and_store(next_candidate);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for offset 0 */

    if (cached_len < (size_t)n) generate_until(n);
    return (seq_t)cached_primes[n - 1];
}
