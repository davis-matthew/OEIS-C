/*
 * A138629  Primes of form 17*n+7.
 * https://oeis.org/A138629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form 17*k + 7
 * Range:     u8 n <= 4, u16 n <= 395, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Small cache of already discovered terms */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t   next_k    = 0;   /* next multiplier k such that 17*k+7 is tested */

/* Simple primality test: works for uint64_t, uint128_t, and arbitrary-precision decodable types */
static bool is_prime_seq(seq_t n)
{
    if (n < 2) return false;
    if ((n & 1) == 0) return n == 2;
    /* Trial divide by odd numbers. The loop bound i*i <= n is safe because
       seq_t supports multiplication and comparison of the same type.       */
    for (seq_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

/* Grow cache capacity when needed */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < cache_cap) new_cap = cache_cap; /* overflow guard */
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = newp;
    cache_cap = new_cap;
}

/* Ensure cache has at least n entries */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* candidate = 17 * k + 7 */
        seq_t cand = ((seq_t)17 * (seq_t)next_k) + 7;
        if (is_prime_seq(cand)) {
            if (cache_len >= cache_cap) grow_cache();
            cache[cache_len++] = cand;
        }
        ++next_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but safe */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    ensure_cache(n);
    return cache[n - 1];
}
