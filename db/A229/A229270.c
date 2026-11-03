/*
 * A229270  Numbers k for which k' - k is prime, k' being the arithmetic
 *          derivative of k.
 * https://oeis.org/A229270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that k′ − k is prime
 * Range:     u8 n <= 2, u16 n <= 219, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 62 terms through n = 492 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isqrt.h"
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

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Global list of primes for trial division */
static uint64_t *g_primes     = NULL;
static size_t   g_prime_cnt   = 0;
static size_t   g_prime_cap   = 0;
static uint64_t g_prime_limit = 0;

/* Extend prime list up to at least 'limit' using a simple sieve. */
static void ensure_primes(uint64_t limit)
{
    if (limit <= g_prime_limit) return;

    size_t n = limit + 1;
    bool *is_prime = (bool *)malloc(n);
    if (!is_prime) exit(1);
    for (size_t i = 0; i < n; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    uint64_t sq = isqrt(limit);
    for (uint64_t i = 2; i <= sq; ++i)
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }

    for (uint64_t i = g_prime_limit + 1; i <= limit; ++i)
        if (is_prime[i]) {
            if (g_prime_cnt == g_prime_cap) {
                size_t new_cap = (g_prime_cap == 0) ? 8 : g_prime_cap << 1;
                uint64_t *tmp = (uint64_t *)realloc(g_primes, new_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                g_primes = tmp;
                g_prime_cap = new_cap;
            }
            g_primes[g_prime_cnt++] = i;
        }

    g_prime_limit = limit;
    free(is_prime);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static bool init = false;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_len = 0;
    static uint64_t last_k = 1;            /* last integer tested */

    if (!init) {
        init = true;
        ensure_cache(1, &cache, &cache_cap);
        cache_len = 0;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        ++last_k;                         /* candidate k */
        uint64_t k = last_k;
        uint64_t temp = k;
        uint64_t sqrtk = isqrt(k);
        ensure_primes(sqrtk);

        seq_t deriv = 0;                  /* arithmetic derivative of k */

        /* Factorise k using the prime table */
        for (size_t i = 0; i < g_prime_cnt && g_primes[i] <= sqrtk; ++i) {
            uint64_t p = g_primes[i];
            if (temp % p == 0) {
                uint64_t exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                deriv += ((seq_t)k) / (seq_t)p * (seq_t)exp;
            }
        }
        if (temp > 1) {   /* remaining prime factor > sqrt(k) */
            deriv += ((seq_t)k) / (seq_t)temp;    /* exponent is 1 */
        }

        /* compare before subtracting: k' < k wrapped around in an unsigned seq_t */
        if (deriv > (seq_t)k + (seq_t)1 && is_prime_big(deriv - (seq_t)k)) {
            ensure_cache(cache_len + 1, &cache, &cache_cap);
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];   /* a(1) corresponds to cache[0] */
}
