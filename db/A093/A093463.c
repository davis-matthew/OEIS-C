/*
 * A093463  a(1) = 1; for n>1, a(n) = n*a(n-1) + 1 if n is a prime else a(n) =
 *          n*a(n-1) - 1.
 * https://oeis.org/A093463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n>1, a(n) = n*a(n-1) + 1 if n is prime else a(n) =
 *            n*a(n-1) - 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* cache of previously computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* allocated size of the cache */
static idx_t cur_max = 0;      /* largest n whose value is cached */

/* list of primes found so far – used for a fast primality test */
static idx_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

static void append_prime(idx_t p) {
    if (primes_len == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_len++] = p;
}

static int is_prime(idx_t n) {
    if (n < 2) return 0;
    for (size_t i = 0; i < primes_len; ++i) {
        idx_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return 0;
    }
    return 1;
}

/* Extend the cache so that it contains values up to index n. */
static void ensure_cache_to(idx_t n) {
    if (n == 0) return;

    /* bootstrap a(1) if this is the first call */
    if (cur_max == 0) {
        size_t init_len = 2;              /* indices 0 and 1 */
        cache = (seq_t *)realloc(cache, init_len * sizeof(seq_t));
        if (!cache) abort();
        cache_len = init_len;
        cache[1] = (seq_t)1;              /* a(1) = 1 */
        cur_max = 1;
    }
    if (n <= cur_max) return;

    /* grow the cache if needed */
    size_t needed = n + 1;                 /* include index 0 */
    if (needed > cache_len) {
        size_t new_len = cache_len ? cache_len * 2 : 16;
        while (new_len <= n) new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = new_len;
    }

    /* compute values for indices cur_max+1 .. n */
    for (idx_t k = cur_max + 1; k <= n; ++k) {
        seq_t prev = cache[k - 1];
        seq_t prod = prev * (seq_t)k;
        if (is_prime(k)) {
            cache[k] = prod + 1;
            append_prime(k);
        } else {
            cache[k] = prod - 1;
        }
    }
    cur_max = n;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* safety value for offset 1 */
    ensure_cache_to(n);
    return cache[n];
}
