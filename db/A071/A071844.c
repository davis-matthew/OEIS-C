/*
 * A071844  Numbers k such that reverse(gpf(k)) = gpf(k+1), where gpf(n) =
 *          A006530(n); a(1)=1.
 * https://oeis.org/A071844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   rev_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A071844 : Numbers k such that reverse(gpf(k)) = gpf(k+1),
   a(1) = 1.  The routine scans successive integers, computes their
   largest prime factor (using a sieve‑generated prime table), reverses
   its decimal digits, and compares it to the gpf of k+1.  All qualifying
   k are cached so that arbitrary calls to a(n) are answered in O(1)
   after the initial sieving.  Division by primes is performed only up to
   the square root of the current number, so the algorithm runs comfortably
   within the required time limits. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/rev_uint.h"

/* --- Prime sieve --------------------------------------------------------- */
#define SIEVE_LIMIT 1000000U   /* primes up to 1,000,000 (≈78498 primes) */

static uint32_t *prime_table    = NULL;   /* list of primes up to SIEVE_LIMIT */
static size_t   prime_count      = 0;     /* number of primes stored */

/* Generate the prime table on first use. */
static void init_primes(void)
{
    if (prime_table != NULL)
        return;

    bool *is_composite = (bool *)calloc(SIEVE_LIMIT + 1, 1);
    if (!is_composite)
        abort();                  /* out of memory */

    /* estimate number of primes to allocate */
    prime_count = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_composite[i]) {
            ++prime_count;
            if (i * i <= SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }

    prime_table = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!prime_table)
        abort();

    size_t idx = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_composite[i])
            prime_table[idx++] = i;

    free(is_composite);
}

/* Largest prime factor of n (n >= 1).  For n <= 1 the function returns 1. */
static uint64_t largest_prime_factor(uint64_t n)
{
    if (n <= 1)
        return 1;

    init_primes();

    uint64_t largest = 1;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_table[i];
        if ((uint64_t)p * (uint64_t)p > n)
            break;
        while (n % p == 0) {
            largest = p;
            n /= p;
        }
    }
    if (n > 1)               /* remainder is a prime > sqrt(original n) */
        largest = n;
    return largest;
}

/* --- Sequence cache ---------------------------------------------------- */
static seq_t *cache          = NULL;   /* cached terms a(1), a(2), ... */
static idx_t  cache_size     = 0;      /* number of cached terms */
static idx_t  cache_capacity = 0;      /* allocated capacity */
static uint64_t next_candidate = 2;    /* next integer to test (after a(1)=1) */

static void grow_cache(idx_t need)
{
    if (need <= cache_capacity)
        return;
    idx_t new_capacity = cache_capacity ? cache_capacity * 2 : 256;
    while (new_capacity < need)
        new_capacity <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
    if (!new_cache)
        abort();
    cache = new_cache;
    cache_capacity = new_capacity;
}

static void init_cache(void)
{
    if (cache != NULL)
        return;
    cache_capacity = 256;
    cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    if (!cache)
        abort();
    cache[0] = (seq_t)1;   /* a(1) = 1 */
    cache_size = 1;
    next_candidate = 2;
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_cache();

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        uint64_t k          = next_candidate;
        uint64_t gpf_k      = largest_prime_factor(k);
        uint64_t rev_gpf_k  = rev_uint(gpf_k);
        uint64_t gpf_kplus  = largest_prime_factor(k + 1);
        if (rev_gpf_k == gpf_kplus) {
            grow_cache(cache_size + 2);   /* ensure space for new term */
            cache[cache_size++] = (seq_t)k;
        }
        ++next_candidate;
    }
    return cache[n - 1];   /* cache is 0‑based, a(1) is cache[0] */
}
