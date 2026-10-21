/*
 * A053706  Primes p such that between p and the next prime, 2 prime powers
 *          (A025475) occur.
 * https://oeis.org/A053706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   is_prime_uint64, pow_leq, root_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A053706:
   a(n) = the nth prime p such that in the interval (p, next_prime(p))
   there are exactly two prime powers (numbers of the form q^k with k ≥ 2). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"
#include "../utils/pow_leq.h"
#include "../utils/root_floor.h"

/* Cached terms of the sequence */
static seq_t *cache = NULL;      /* array of stored terms */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t  cache_len = 0;     /* number of terms stored */

/* Ensure the cache has room for at least 'need' terms */
static void ensure_cache(idx_t need)
{
    if ((size_t)need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while ((size_t)need > new_cap) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { exit(EXIT_FAILURE); }
    cache = tmp;
    cache_cap = new_cap;
}

/* Next prime after n (n ≥ 2). */
static uint64_t next_prime_uint64(uint64_t n)
{
    uint64_t cand = n + 1;
    while (!is_prime_uint64(cand)) ++cand;
    return cand;
}

/* Count how many prime powers lie strictly between p and q (p < q). */
static int count_prime_powers(uint64_t p, uint64_t q)
{
    if (q <= p + 1) return 0;          /* no space for any number */
    int cnt = 0;
    uint64_t max_val = q - 1;          /* largest number we may consider */
    for (int exp = 2; ; ++exp) {
        if (!pow_leq(2, exp, max_val)) break;   /* 2^exp > max_val : no more */
        uint64_t lo = root_floor(p, exp) + 1;   /* smallest base with b^exp > p */
        uint64_t hi = root_floor(max_val, exp); /* largest base with b^exp <= max_val */
        if (lo > hi) continue;
        for (uint64_t b = lo; b <= hi; ++b) {
            if (is_prime_uint64(b)) {
                ++cnt;
                if (cnt > 2) return cnt;      /* early exit */
            }
        }
    }
    return cnt;
}

/* Ensure that at least 'n' terms of the sequence are computed and cached. */
static void ensure_terms(idx_t n)
{
    if (cache_len >= n) return;

    uint64_t p;
    /* Start from the prime following the last cached term, if any */
    if (cache_len == 0) {
        p = 2;
    } else {
        p = next_prime_uint64((uint64_t)cache[cache_len - 1]);
    }

    for (;;) {
        uint64_t q = next_prime_uint64(p);
        if (count_prime_powers(p, q) == 2) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)p;
            if (cache_len >= n) return;
        }
        p = q;   /* move to the next gap */
    }
}

/* Public function requested by the platform */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n = 0; defensive */
    ensure_terms(n);
    return cache[n - 1];
}
