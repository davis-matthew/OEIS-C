/*
 * A133559  Primes which have a partition as the sum of squares of five
 *          consecutive primes.
 * https://oeis.org/A133559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime that equals the sum of the squares of five
 *            consecutive primes
 * Range:     u8 none, u16 n <= 9, u32 n <= 615, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Helper: fast deterministic Miller–Rabin for 64‑bit integers       */
/* ------------------------------------------------------------------ */
static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static inline uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t result = 1;
    while (e) {
        if (e & 1) result = mod_mul(result, a, mod);
        a = mod_mul(a, a, mod);
        e >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Prime generation (dynamic sieve)                                  */
/* ------------------------------------------------------------------ */
static size_t sieve_limit = 5000000;          /* start with 5 M */
static uint64_t *primes_arr = NULL;
static size_t primes_cnt = 0;
static bool primes_init = false;

static void recompute_primes(size_t limit)
{
    bool *sieve = (bool*)malloc(limit + 1);
    memset(sieve, 1, limit + 1);
    sieve[0] = sieve[1] = 0;
    for (size_t i = 2; i * i <= limit; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = 0;

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;

    uint64_t *buf = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) buf[idx++] = (uint64_t)i;

    free(primes_arr);
    primes_arr = buf;
    primes_cnt = cnt;
    sieve_limit = limit;
    free(sieve);
}

static void ensure_primes(size_t needed)
{
    if (primes_cnt >= needed) return;
    size_t newlimit = sieve_limit;
    while (primes_cnt < needed) {
        newlimit *= 2;
        recompute_primes(newlimit);
    }
}

static void init_primes(void)
{
    if (primes_init) return;
    recompute_primes(sieve_limit);
    primes_init = true;
}

/* ------------------------------------------------------------------ */
/*  Cache for found sums                                              */
/* ------------------------------------------------------------------ */
static uint64_t *cache_arr = NULL;
static size_t cache_cnt = 0;
static size_t cache_cap = 0;
static size_t next_window = 0;
static bool cache_init = false;

static void grow_cache(void)
{
    cache_cap = cache_cap ? 2 * cache_cap : 64;
    cache_arr = (uint64_t*)realloc(cache_arr, cache_cap * sizeof(uint64_t));
}

static void init_cache(void)
{
    if (cache_init) return;
    cache_cap = 64;
    cache_arr = (uint64_t*)malloc(cache_cap * sizeof(uint64_t));
    cache_cnt = 0;
    next_window = 0;
    cache_init = true;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0; return 0 to avoid UB */
    init_primes();
    init_cache();

    while (cache_cnt < (size_t)n) {
        /* make sure there are primes for the next window */
        if (next_window + 5 > primes_cnt) {
            ensure_primes(next_window + 5);
        }

        uint64_t p0 = primes_arr[next_window];
        uint64_t p1 = primes_arr[next_window + 1];
        uint64_t p2 = primes_arr[next_window + 2];
        uint64_t p3 = primes_arr[next_window + 3];
        uint64_t p4 = primes_arr[next_window + 4];

        uint64_t sum = p0 * p0 + p1 * p1 + p2 * p2 + p3 * p3 + p4 * p4;

        if (is_prime_uint64(sum)) {
            if (cache_cnt == cache_cap) grow_cache();
            cache_arr[cache_cnt++] = sum;
        }
        ++next_window;
    }

    return (seq_t)cache_arr[(size_t)n - 1];
}
