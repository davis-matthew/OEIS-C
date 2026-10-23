/*
 * A031906  a(n) = prime(6*n - 5).
 * https://oeis.org/A031906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(6*n-5)
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Cache of primes up to a fixed limit.  The limit is large enough for
   n up to about 10000 (k = 6*n-5 ≈ 60000, the 60000‑th prime ≈ 770003).   */
static uint64_t *prime_cache = NULL;   /* pointer to array of primes      */
static size_t   prime_count  = 0;      /* number of primes stored         */
static int      sieve_done   = 0;      /* flag whether caching is done    */

/* Build the prime table once, up to LIMIT.  The table is indexed
   from 0 (prime 2) upwards. */
static void build_prime_cache(void)
{
    if (sieve_done) return;

    /* Integer limit for sieve.  For n≤10000 1,000,000 is ample. */
    const uint64_t LIMIT = 1000000ull;

    /* Allocate boolean array: 0/1 flag for primes.  Cast for C++ */
    char *is_prime = (char *)calloc((size_t)(LIMIT + 1), 1);
    if (!is_prime) abort();

    /* Mark all numbers ≥ 2 as potentially prime. */
    for (uint64_t i = 2; i <= LIMIT; ++i) is_prime[i] = 1;

    /* Eratosthenes sieve */
    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) ++cnt;
    }

    /* Allocate array for primes */
    prime_cache = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_cache) abort();

    /* Store primes */
    size_t pos = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            prime_cache[pos++] = i;
        }
    }
    prime_count = cnt;
    free(is_prime);
    sieve_done = 1;
}

/* Return the k-th prime (k is 1‑based).  The caller must ensure
   k ≤ prime_count (true for n≤10000 with our limit). */
static seq_t prime_k(idx_t k)
{
    if (!sieve_done) build_prime_cache();
    if (k > prime_count) abort();          /* out‑of‑range guard */
    return (seq_t)prime_cache[k - 1];       /* cast to seq_t */
}

/* ------------------------------------------------------------------ */
/* Sequence generator -------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Compute the index of the required prime: k = 6*n-5.       */
    idx_t k = 6 * n - 5;                     /* 6 and 5 are small, no overflow for n≤10⁴ */
    return prime_k(k);
}
