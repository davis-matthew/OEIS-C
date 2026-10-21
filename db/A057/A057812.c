/*
 * A057812  Numbers k such that pi(k) is odd.
 * https://oeis.org/A057812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th element of OEIS A057812:
   a(n) = the n‑th integer k for which π(k) is odd.
   π(k) is the number of primes ≤ k.
   The sequence consists of all integers lying in the intervals
   [p_{2m‑1}, p_{2m} − 1] where p_i denotes the i-th prime.  */
#include <stdint.h>
#include <stdlib.h>

/* Cached values of the sequence */
static seq_t *cache = NULL;           /* array of seq_t values */
static size_t  cache_size = 0;        /* number of cached terms */
static size_t  cache_capacity = 0;    /* allocated capacity */

/* List of known primes, used for fast primality tests */
static uint64_t *primes_arr = NULL;   /* array of uint64_t primes */
static size_t   primes_count = 0;     /* number of stored primes */
static size_t   primes_capacity = 0;  /* allocated capacity */

/* State that is continued across calls */
static idx_t    current_k = 1;        /* last number examined (starts at 1) */
static idx_t    pi = 0;               /* current value of π(current_k) */

/* ------------------------------------------------------------------ */
/* Helper: grow the cache array if necessary */
static void
ensure_cache(size_t needed)
{
    if (needed > cache_capacity) {
        size_t new_cap = cache_capacity ? 2 * cache_capacity : 16;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* out of memory */
        cache = tmp;
        cache_capacity = new_cap;
    }
}

/* Helper: grow the prime list array if necessary */
static void
ensure_primes(size_t needed)
{
    if (needed > primes_capacity) {
        size_t new_cap = primes_capacity ? 2 * primes_capacity : 16;
        while (new_cap < needed)
            new_cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(primes_arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        primes_arr = tmp;
        primes_capacity = new_cap;
    }
}

/* Primitive trial‑division primality test using the known primes list.
   For n < 2 the result is false. */
static int
is_prime_uint64(uint64_t n)
{
    if (n < 2) return 0;
    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes_arr[i];
        if (p > n / p) break;           /* p * p > n without overflow  */
        if (n % p == 0) return 0;
    }
    return 1;
}

/* ------------------------------------------------------------------ */
/* Main function requested by the build runners */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n = 0 – defensive */

    /* Initialise static storage on first call */
    if (!cache) {
        cache_capacity = 0;
        cache = NULL;
        cache_size = 0;
        primes_capacity = 0;
        primes_arr = NULL;
        primes_count = 0;
        current_k = 1;
        pi = 0;
    }

    /* Generate terms until we have at least n terms cached */
    while (cache_size < (size_t)n) {
        ++current_k;                    /* examine the next integer */

        int prime = is_prime_uint64(current_k);
        if (prime) {
            ensure_primes(primes_count + 1);
            primes_arr[primes_count++] = current_k;
            ++pi;
        }

        if (pi & 1) {                   /* π(k) odd → include k in the sequence */
            ensure_cache(cache_size + 1);
            cache[cache_size++] = (seq_t)current_k;
        }
    }

    return cache[n - 1];
}
