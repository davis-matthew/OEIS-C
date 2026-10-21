/*
 * A073781  Number of 2's in base-3 representation of n-th prime.
 * https://oeis.org/A073781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of digit 2's in the base‑3 representation of the
 *            nth prime. a(1) is the first term. Index n is 1‑based, any n ≥ 1
 *            is accepted,\ n == 0 yields 0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a static list of primes and expands it
   lazily until it contains the requested nth prime.  The primes are
   stored in a 32‑bit unsigned array because the 10 000th prime
   (104729) easily fits into 32 bits; the algorithm works for any
   n that fits in 64 bits.  All arithmetic uses 64‑bit intermediates
   to avoid overflow when testing divisibility. */
#include <stdint.h>
#include <stdlib.h>

static uint32_t *prime_cache = NULL;     /* stored primes */
static size_t   prime_cnt   = 0;         /* number of primes stored */
static size_t   prime_cap   = 0;         /* capacity of prime_cache */
static uint32_t next_candidate = 2;      /* next number to test for primality */

/* Grow the prime_cache array.  The growth factor is 2. */
static void grow_prime_cache(void)
{
    size_t new_cap = prime_cap ? prime_cap * 2 : 64;
    uint32_t *new_p = (uint32_t *)realloc(prime_cache, new_cap * sizeof(uint32_t));
    if (!new_p) abort();               /* out‑of‑memory protection */
    prime_cache = new_p;
    prime_cap   = new_cap;
}

/* Test whether x is prime using the current prime_cache (dividing only
   by primes ≤ sqrt(x)).  This function assumes that prime_cache
   contains all primes ≤ sqrt(x). */
static int is_prime_candidate(uint32_t x)
{
    if (x < 2) return 0;
    uint64_t limit = (uint64_t)x * (uint64_t)x; /* compare p*p <= x */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint32_t p = prime_cache[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > limit) break;
        if (x % p == 0) return 0;
    }
    return 1;
}

/* Ensure that prime_cache contains at least n primes (n ≥ 1). */
static void ensure_primes(idx_t n)
{
    while (prime_cnt < n)
    {
        /* Ensure the cache is large enough. */
        if (prime_cnt == prime_cap) grow_prime_cache();

        if (is_prime_candidate(next_candidate))
        {
            /* Store the prime. */
            prime_cache[prime_cnt++] = next_candidate;
        }

        /* Advance to the next candidate. */
        if (next_candidate == 2)
            next_candidate = 3; /* only odd numbers after 2 are needed */
        else
            next_candidate += 2;
    }
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n==0, return 0 */
    ensure_primes(n);                /* make sure we have at least n primes */

    uint32_t nth_prime = prime_cache[n - 1];   /* primes[0] is the first prime */
    seq_t count = 0;
    while (nth_prime > 0)
    {
        if ((nth_prime % 3u) == 2u) ++count;
        nth_prime /= 3u;
    }
    return count;
}
