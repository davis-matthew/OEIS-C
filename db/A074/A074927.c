/*
 * A074927  a(n) such that p(n)*p(n+1)+a(n) is a minimal square.
 * https://oeis.org/A074927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((p(n+1)-p(n))/2)^2 for n > 1, a(1) = 3 (p(n) = n-th
 *            prime)
 * Range:     u8 n <= 216, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Prime generation helpers -------------------------------------------------- */

/* Dynamically sized array of 64‑bit primes.  The array is expanded only as
   more primes are required.  The primes are stored in increasing order.   */
static uint64_t *prime_cache = NULL;   /* array of primes */
static size_t    prime_count = 0;     /* number of primes stored */
static size_t    prime_cap   = 0;     /* allocated capacity */
static uint64_t  next_candidate = 3;  /* next odd number to test */

/*--- Simple deterministic 64‑bit primality test --------------------------------
  Uses trial division by all cached primes up to sqrt(n).  This is more than
  fast enough for generating the first 100 000 primes (~1.3 million).    */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_cache[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Expand the prime cache to contain at least (needed+1) primes
   (index 0..needed).  The cache grows geometrically. */
static void ensure_primes(idx_t needed)
{
    if (prime_count > needed) return;          /* already enough */

    /* Initialise the cache with the first prime */
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_cache = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    }
    if (prime_count == 0) {
        prime_cache[prime_count++] = 2;        /* p(1) = 2 */
    }

    /* Append primes until we have at least needed+1 primes */
    while (prime_count <= needed) {
        /* grow capacity if necessary */
        if (prime_count == prime_cap) {
            prime_cap <<= 1;
            prime_cache = (uint64_t*)realloc(prime_cache, prime_cap * sizeof(uint64_t));
        }

        /* find next prime */
        while (!is_prime_uint64(next_candidate)) {
            next_candidate += 2;                /* skip evens */
        }
        prime_cache[prime_count++] = next_candidate;
        next_candidate += 2;
    }
}

/* Main sequence function ---------------------------------------------------- */

seq_t a(idx_t n)
{
    /* sequence is defined for n >= 1; for n==0 return 0 as safety */
    if (n == 0) return (seq_t)0;

    /* a(1) is given explicitly */
    if (n == 1) return (seq_t)3;

    /* Need primes p(n) and p(n+1), i.e. primes[n-1] and primes[n] */
    ensure_primes(n);                          /* primes[n] will exist */

    uint64_t p_n   = prime_cache[n - 1];        /* p(n)   */
    uint64_t p_n1  = prime_cache[n];          /* p(n+1) */
    uint64_t diff  = p_n1 > p_n ? p_n1 - p_n : 0;     /* always even for n>1 */

    uint64_t half  = diff / 2;
    uint64_t res   = half * half;              /* fits easily into 64‑bit */

    return (seq_t)res;
}
