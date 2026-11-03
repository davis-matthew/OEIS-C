/*
 * A207990  Primes of the form prime(n) + prime(n+1) - 5.
 * https://oeis.org/A207990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 744, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 67 terms through n = 2784 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A207990 – primes of the form prime(n)+prime(n+1)-5
 *  Formula: a(n) = prime(n) + prime(n+1) - 5
 *  Returned only if the result is prime; otherwise omitted.
 *  The function yields the nth such prime for n ≥ 1.
 *
 *  Implementation strategy:
 *    * Generate a large list of ordinary primes once using
 *      build_primes_upto().
 *    * Scan consecutive prime pairs and form candidate = p[i] + p[i+1] - 5.
 *    * Test each candidate with is_prime_big(); if prime, store in cache.
 *    * Memoize results: subsequent calls resume from the last used
 *      prime index, so the cost is shared.
 *    * 20 000 000 is ample to produce more than 10 000 terms, ensuring
 *      that all requested indices up to the limit of the tests are
 *      available.                                            */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

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

seq_t a(idx_t n)
{
    /* Local static state */
    static uint64_t *primes   = NULL;      /* array of primes ≤ LIMIT */
    static size_t    primes_cnt = 0;       /* number of primes in @primes       */
    static seq_t    *cache   = NULL;      /* cached sequence values            */
    static size_t    cache_len = 0;        /* length of @cache                  */
    static size_t    cache_cap = 0;        /* capacity of @cache                */
    static size_t    cur_pair   = 0;       /* index of next prime pair to process*/

    /* Build the prime list lazily on the first call.  20 000 000 is
     * high enough to guarantee >10 000 valid terms (≈74 000).     */
    if (!primes) {
        const idx_t LIMIT = 20000000ULL;
        build_primes_upto(LIMIT, &primes, &primes_cnt);
    }

    /* Extend the cache until it contains the desired index.     */
    while (cache_len < (size_t)n && cur_pair + 1 < primes_cnt) {
        seq_t cand = (seq_t)primes[cur_pair] +
                     (seq_t)primes[cur_pair + 1] - (seq_t)5;
        if (is_prime_big(cand)) {
            append_candidate(&cache, &cache_len, &cache_cap, cand);
        }
        ++cur_pair;
    }

    /* The problem guarantees that n will be within the bounds of the
     * pre‑computed cache.  If not, return 0 (should never happen). */
    if (n == 0 || cache_len < (size_t)n) return 0;
    return cache[n - 1];
}
