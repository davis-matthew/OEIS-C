/*
 * A067759  Numbers k such that sigma(prime(k) + 1) == 0 (mod k).
 * https://oeis.org/A067759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067759: a(n) = the index k such that sigma(p_k + 1) ≡ 0 (mod k),
   where p_k is the k‑th prime. This implementation precomputes a sieve of
   primes up to a fixed upper bound and checks each successive k until the
   requested term is found. It uses only integer arithmetic compatible
   with any unsigned seq_t type (8–128‑bit or arbitrary precision). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* --------------------------------------------------------------------------
   Prime sieve – built once, up to a fixed limit (20 000 000).  All primes
   are stored in a static array prime_list, and the count is kept in
   prime_count.  A simple Eratosthenes sieve is used; no additional math
   functions are called, avoiding the need for <math.h>.
   -------------------------------------------------------------------------- */

#define PRIME_SIEVE_LIMIT 20000000ULL

static uint32_t *prime_list = NULL;
static size_t      prime_count = 0;
static bool        primes_built = false;

static void build_primes(void)
{
    if (primes_built)
        return;

    /* Allocate the boolean sieve array */
    uint8_t *is_prime = (uint8_t *)malloc(PRIME_SIEVE_LIMIT + 1);
    if (!is_prime)
        return;                       /* allocation failed – caller will fail */

    memset(is_prime, 1, PRIME_SIEVE_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    /* Determine the maximum i such that i * i <= PRIME_SIEVE_LIMIT.
       Avoid calling sqrt() by using a simple loop. */
    size_t max_i = 1;
    while ((max_i + 1) * (max_i + 1) <= PRIME_SIEVE_LIMIT)
        ++max_i;

    /* Classic Eratosthenes sieve */
    for (size_t i = 2; i <= max_i; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count the primes first */
    size_t count = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i]) ++count;

    /* Allocate the prime list */
    prime_list = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!prime_list) {
        free(is_prime);
        return;
    }

    /* Fill the array */
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint32_t)i;

    prime_count = count;
    primes_built = true;
    free(is_prime);
}

/* --------------------------------------------------------------------------
   Sum of divisors σ(n) for n up to PRIME_SIEVE_LIMIT+1.  We factor n using
   the precomputed prime list.  The algorithm is fully integer‑arithmetic
   and works for any seq_t type.
   -------------------------------------------------------------------------- */

static seq_t sigma_of_uint32(uint32_t n)
{
    uint32_t temp = n;
    seq_t result = 1;

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= temp; ++i) {
        uint32_t p = prime_list[i];
        if (temp % p == 0) {
            seq_t sum_p = 1;
            seq_t power = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= (seq_t)p;
                sum_p += power;
            }
            result *= sum_p;
        }
    }

    if (temp > 1) {
        result *= ((seq_t)temp + 1);
    }

    return result;
}

/* --------------------------------------------------------------------------
   Cache of already found terms of the sequence.  The cache is
   extended as needed; static so that repeated calls with different n
   values are efficient.
   -------------------------------------------------------------------------- */

static seq_t *term_cache   = NULL;
static size_t term_cap     = 0;
static size_t term_len     = 0;        /* number of cached terms */
static idx_t  next_k_check = 1;        /* next k to test */

/* --------------------------------------------------------------------------
   Main function required by the build.  Computes the n‑th term of the
   sequence.  The function works for any build where seq_t and idx_t are
   defined appropriately.  It tolerates out‑of‑order calls and builds
   the cache lazily until the required term is found.
   -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)                /* out of range – defensive return */
        return 0;

    if (!primes_built)
        build_primes();

    /* Extend the cache until it contains at least n terms. */
    while (term_len < (size_t)n) {
        idx_t k = next_k_check++;
        if ((size_t)(k - 1) >= prime_count) {
            /* We ran out of precomputed primes – this should not
               happen for the intended usage (n up to about 10 000).  */
            return 0;
        }

        seq_t pk  = (seq_t)prime_list[(size_t)(k - 1)];
        seq_t x   = pk + 1;
        seq_t sig = sigma_of_uint32((uint32_t)x);

        if (sig % (seq_t)k == 0) {
            /* Ensure room in the cache. */
            if (term_len == term_cap) {
                term_cap = term_cap == 0 ? 16 : term_cap * 2;
                term_cache = (seq_t *)realloc(term_cache, term_cap * sizeof(seq_t));
                if (!term_cache)
                    return 0;   /* allocation failure */
            }
            term_cache[term_len++] = k;
        }
    }

    return term_cache[(size_t)(n - 1)];
}
