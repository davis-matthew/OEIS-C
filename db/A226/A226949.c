/*
 * A226949  Number of twin prime pairs of the form k*n +/- 1 with k <= n.
 * https://oeis.org/A226949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { 1 <= k <= n | both k*n-1 and k*n+1 are prime }
 * Range:     u8 n <= 2729, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: precompute primes up to the required limit using a bit‑sieve and
   cache the sequence values. This allows O(1) lookup after O(max_n²) preprocessing. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* The following types are defined by the build system. */

/* Sequence element type. */
typedef seq_t seq_t;
/* Index type. */
typedef idx_t idx_t;

/* Static storage for the sequence cache and prime sieve. */
static seq_t *a_cache = NULL;           /* a_cache[i] holds a(i) for 1 <= i <= cache_size. */
static idx_t  cache_size = 0;           /* Highest n that has been computed. */

static unsigned char *prime_bits = NULL;/* Bitset: 1 = prime, 0 = composite. */
static idx_t        prime_bits_limit = 0;/* Largest number for which prime_bits is valid. */

/* Test whether x is prime using the prime_bits bitset. */
static inline bool is_prime_bit(idx_t x)
{
    if (x > prime_bits_limit) return false;   /* Should not happen. */
    return (prime_bits[x >> 3] >> (x & 7)) & 1;
}

/* Build a new prime bitset up to (inclusive) 'limit'.  All old memory is freed. */
static void build_prime_bits(idx_t limit)
{
    idx_t byte_len = (limit >> 3) + 1;
    unsigned char *new_bits = (unsigned char *)malloc(byte_len);
    if (!new_bits) { /* In a re‑implementation environment this shouldn't happen. */ }
    memset(new_bits, 0xFF, byte_len);           /* Assume all are prime. */
    if (limit >= 0) new_bits[0] &= ~(1 << 0);    /* 0 is composite. */
    if (limit >= 1) new_bits[0] &= ~(1 << 1);    /* 1 is composite. */

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime_bit(p)) {
            for (idx_t m = p * p; m <= limit; m += p)
                new_bits[m >> 3] &= ~(1u << (m & 7));
        }
    }

    free(prime_bits);
    prime_bits = new_bits;
    prime_bits_limit = limit;
}

/* Ensure we have a prime bitset covering up to (n*n + 1). */
static void ensure_prime_bits(idx_t n)
{
    idx_t required = n * n + 1;
    if (required <= prime_bits_limit) return;
    build_prime_bits(required);
}

/* Ensure the sequence cache covers up to (n).  Newly added entries are computed. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;

    /* Extend the cache array. */
    idx_t new_cap = n + 1; /* +1 because we use 1‑based indexing */
    seq_t *new_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_cache) { /* In a re‑implementation environment this shouldn't happen. */ }
    a_cache = new_cache;

    /* Compute sequence values for indices beyond the old cache size. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t count = 0;
        for (idx_t k = 1; k <= i; ++k) {
            idx_t p1 = i * k - 1;
            idx_t p2 = i * k + 1;
            if (is_prime_bit(p1) && is_prime_bit(p2))
                ++count;
        }
        a_cache[i] = count;
    }
    cache_size = n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* Not part of the OEIS offset, but safe. */

    ensure_prime_bits(n);
    ensure_cache(n);
    return a_cache[n];
}
