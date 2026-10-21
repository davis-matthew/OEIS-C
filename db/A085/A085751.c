/*
 * A085751  Semiprimes whose reversal is also a semiprime.
 * https://oeis.org/A085751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A085751 – Semiprimes whose decimal reversal is also a semiprime. */
/* a(n) gives the nth such number (1‑based). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/rev.h"   /* rev(idx_t)            */

static seq_t *cache      = NULL; /* cached sequence values      */
static size_t cache_size = 0;    /* number of cached values     */
static size_t cache_cap   = 0;    /* allocated capacity          */
static idx_t  next_candidate = 4;/* next integer to test        */

/* Ensure that cache can hold at least N elements.  Resizes if necessary. */
static void ensure_cache(size_t N)
{
    if (cache_size >= N) return;
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_cap < N) {
        cache_cap <<= 1;
    }
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

/* Simple primality test for 64‑bit values. */
static bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Return the smallest prime factor of n. */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if ((n & 1) == 0) return 2;
    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0) return d;
    }
    return n;                      /* n is prime itself */
}

/* Return true if x is a semiprime: product of exactly two primes. */
static bool is_semiprime(uint64_t x)
{
    if (x < 4) return false;                    /* < 2*2 cannot be semiprime */
    uint64_t sp = smallest_prime_factor(x);
    if (sp == x) return false;                 /* x is prime itself */
    uint64_t q = x / sp;
    return is_prime(q);                         /* q must be prime */
}

/* Generate sequence terms up to index N (1‑based).  Cache is expanded as
   needed and tests all integers in ascending order. */
static void generate_up_to(size_t N)
{
    if (N <= cache_size) return;
    ensure_cache(N);
    while (cache_size < N) {
        if (is_semiprime((uint64_t)next_candidate)) {
            idx_t rev_val = rev((idx_t)next_candidate);
            if (is_semiprime((uint64_t)rev_val)) {
                cache[cache_size++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }
}

/* External sequence function.  Returns a(n) for n ≥ 1, zero for n==0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for index 0 */
    generate_up_to((size_t)n);
    return cache[(size_t)n - 1];
}
