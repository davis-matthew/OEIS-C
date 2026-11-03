/*
 * A215992  Primes that are the sum of 51 consecutive primes.
 * https://oeis.org/A215992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 48, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 51 terms through n = 9824 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes that are the sum of 51 consecutive primes.
   The algorithm builds an increasing list of primes, slides a
   51‑term window over it, and records the window sums that are prime.
   The cache is extended lazily until the requested index is known. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Static state */
static uint64_t *primes      = NULL;   /* prime list */
static size_t   prime_cnt   = 0;       /* number of primes */
static seq_t   *cache       = NULL;   /* cached sums that are prime */
static idx_t    cache_len   = 0;       /* number of terms stored */
static size_t   cache_cap   = 0;       /* capacity of cache */
static idx_t    prime_limit = 0;       /* current sieve limit */

/* Rebuild primes and cache from scratch for the current `prime_limit`. */
static void recompute(void)
{
    /* Build new prime list */
    free(primes);     /* may be NULL on first call */
    build_primes_upto(prime_limit, &primes, &prime_cnt);

    /* Reset cache */
    mem_free(cache);
    cache_cap  = 1024;
    cache_len  = 0;
    cache = mem_new(seq_t, cache_cap);

    if (prime_cnt < 51)                /* window too small to evaluate */
        return;

    /* Initialize sliding‑window sum of the first 51 primes */
    seq_t sum = 0;
    for (idx_t i = 0; i < 51; ++i)
        sum += (seq_t)primes[i];

    /* Scan all 51‑term windows */
    for (idx_t k = 0; k + 51 <= prime_cnt; ++k)
    {
        if (is_prime_big(sum))           /* record if sum is prime */
        {
            if (cache_len == cache_cap)  /* grow cache if needed */
            {
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = sum;
        }

        /* Advance the window by one prime, if possible */
        if (k + 51 < prime_cnt)
        {
            sum += (seq_t)primes[k + 51];
            sum -= (seq_t)primes[k];
        }
    }
}

/* Make sure `cache_len >= n`.  If not, increase the sieve limit
   (doubling each time) and rebuild the cache until enough terms are
   found.  The process is safe even if `n` is small or large. */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;         /* index zero has no defined value */

    while (cache_len < n)
    {
        if (prime_limit == 0)    /* start with a moderate limit */
            prime_limit = 2000000;
        else
            prime_limit <<= 1;    /* double the limit each iteration */

        recompute();
    }
}

/* Public sequence function.  Calls `ensure_cache` to guarantee that
   the result is available for any call order. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n == 0 || n > cache_len)  /* out of bounds (should not happen) */
        return (seq_t)0;
    return cache[n - 1];
}
