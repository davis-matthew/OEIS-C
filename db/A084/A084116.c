/*
 * A084116  Numbers m such that A084115(m) = 1.
 * https://oeis.org/A084116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m such that tau(m)^tau(m)+1 is prime,
 *            where tau(m) is the divisor function A000005(m)
 * Range:     u8 n <= 3, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, is_prime_big, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds an SPF sieve incrementally and computes
   divisor counts from it.  For each candidate m it tests whether
   tau(m)^tau(m)+1 is prime using is_prime_big.  The sequence is
   stored in a static cache so that calls in any order are supported. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_spf.h"
#include "../utils/seq_pow.h"   /* provides seq_pow(seq_t, idx_t)     */
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* cached sequence values      */
static size_t cache_len = 0;         /* number of cached values     */
static size_t cache_cap = 0;         /* current allocation capacity */
static idx_t  next_m = 2;            /* next integer to examine    */

static idx_t *spf = NULL;            /* smallest prime factor array */
static idx_t  spf_limit = 0;         /* current size of spf array   */

/* Ensure the SPF array covers numbers up to 'limit'. */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit) return;
    if (spf) free(spf);
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* Append a value to the cache, resizing if needed. */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? 2 * cache_cap : 16;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Compute tau(n) from the SPF array. */
static idx_t tau_from_spf(idx_t n)
{
    if (n == 1) return 1;
    idx_t result = 1;
    while (n > 1) {
        idx_t p = spf[n];
        idx_t exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        result *= (exp + 1);
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    while (cache_len < n) {
        /* Expand sieve as needed. */
        ensure_spf(next_m);

        /* Compute tau(next_m). */
        idx_t tau = tau_from_spf(next_m);

        /* test tau^tau + 1. */
        seq_t cand = seq_pow((seq_t)tau, tau);
        cand = cand + (seq_t)1;
        if (is_prime_big(cand))
            append_cache((seq_t)next_m);

        ++next_m;
    }
    return cache[n - 1];
}
