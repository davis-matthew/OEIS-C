/*
 * A132399  Number of ordered ways of writing n = i + j, where i is 0 or a
 *          prime and j is a triangular number (A000217) >= 0.
 * https://oeis.org/A132399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of ordered ways of writing n = i + j,
 *            where i is 0 or a prime and j is a triangular number (A000217) >= 0.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- Integer square‑root for 64‑bit values ---------- */
static inline idx_t int_sqrt(idx_t n)
{
    /* Integer square‑root using a binary approach (fast enough for n <= 2^64). */
    idx_t low = 0, high = (idx_t)1ULL << 32;   /* high^2 >= n for all 64‑bit n */
    idx_t mid;
    while (low <= high) {
        mid = (low + high) >> 1;
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* ---------- Check if an integer is triangular ---------- */
static inline bool is_triangular(idx_t x)
{
    /* j is triangular iff 8*j+1 is a perfect square. */
    idx_t s = 8 * x + 1;
    idx_t r = int_sqrt(s);
    return r * r == s;
}

/* ---------- Prime sieve up to a given limit ---------- */
static uint32_t *primes_arr = NULL;
static size_t    primes_cnt  = 0;
static idx_t    prime_limit = 0;

static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* Simple sieve; reallocates the prime array every time the limit increases
       (efficient enough for limits up to a few million). */
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime) return;   /* allocation failure -> leave empty */

    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;

    idx_t sq_lim = int_sqrt(limit);
    for (idx_t i = 2; i <= sq_lim; ++i) {
        if (!is_prime[i]) continue;
        for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = false;
    }

    size_t new_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++new_cnt;

    uint32_t *new_primes = (uint32_t *)malloc(new_cnt * sizeof(uint32_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) new_primes[pos++] = (uint32_t)i;

    free(is_prime);
    free(primes_arr);
    primes_arr = new_primes;
    primes_cnt  = new_cnt;
    prime_limit = limit;
}

/* ---------- Cache for a(n) values ---------- */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;

/* Ensure that the cache contains values up to n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
    if (!new_cache) return;   /* allocation failure -> leave empty */

    /* Copy existing cached values. */
    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    idx_t old_size = cache_size;
    cache_size = new_size;

    /* Compute values for newly added indices. */
    for (idx_t k = old_size; k <= n; ++k) {
        seq_t count = 0;

        /* i = 0 case */
        if (is_triangular(k)) ++count;

        /* i = prime cases */
        ensure_primes_upto(k);
        for (size_t pi = 0; pi < primes_cnt; ++pi) {
            uint32_t p = primes_arr[pi];
            if (p > k) break;
            idx_t j = k - (idx_t)p;
            if (is_triangular(j)) ++count;
        }
        cache[k] = count;
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
