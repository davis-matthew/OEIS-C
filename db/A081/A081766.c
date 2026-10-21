/*
 * A081766  Numbers k such that k^2+1 divides C(2k,k).
 * https://oeis.org/A081766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 5169, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  Numbers k such that k^2+1 divides C(2k,k) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Prime sieve */
#define PRIME_LIMIT 200000
static uint32_t primes_arr[30000];
static size_t primes_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    size_t n = PRIME_LIMIT;
    bool *sieve = (bool*)malloc(n + 1);
    if (!sieve) exit(1);
    memset(sieve, 0, n + 1);
    for (size_t i = 2; i <= n; ++i) {
        if (!sieve[i]) {
            primes_arr[primes_cnt++] = (uint32_t)i;
            if (i * i <= n) {
                for (size_t j = i * i; j <= n; j += i) sieve[j] = true;
            }
        }
    }
    free(sieve);
    primes_ready = true;
}

/* Legendre exponent of prime p in C(2k,k) */
static int exp_in_binom(idx_t k, uint32_t p)
{
    int v = 0;
    uint64_t pk = p;
    uint64_t dk = 2 * k;
    while (pk <= dk) {
        v += dk / pk;
        v -= 2 * (k / pk);
        if (pk > dk / p) break;
        pk *= p;
    }
    return v;
}

/* Test if k satisfies k^2+1 | C(2k,k) */
static bool is_good(idx_t k)
{
    if (k == 0) return true;          /* 0^2+1 = 1 divides everything */

    unsigned __int128 sq = (unsigned __int128)k * k + 1;
    uint64_t m = (uint64_t)sq;        /* fits 64‑bit for all relevant k */

    uint64_t temp = m;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes_arr[i];
        if ((uint64_t)p * p > temp) break;
        if (temp % p == 0) {
            int need = 0;
            while (temp % p == 0) {
                temp /= p;
                ++need;
            }
            if (exp_in_binom(k, p) < need) return false;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        uint32_t p = (uint32_t)temp;
        if (exp_in_binom(k, p) == 0) return false;
    }
    return true;
}

/* Dynamic cache for sequence values */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t  next_k     = 0;        /* next k to test */

/* Ensure cache has space for at least need capacity */
static void grow_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialize once */
    static bool init_done = false;
    if (!init_done) {
        init_primes();
        init_done = true;
    }

    /* generate terms until we have n of them */
    while (cache_len < (size_t)n) {
        if (is_good(next_k)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
