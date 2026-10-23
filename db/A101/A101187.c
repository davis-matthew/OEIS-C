/*
 * A101187  Values of m for which (6m+1)(12m+1)(18m+1) is a Carmichael number.
 * https://oeis.org/A101187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 692, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n‑th integer m>0 such that (6m+1)(12m+1)(18m+1) is a Carmichael number. */

#if !defined(_GNU_SOURCE) && defined(__linux__)
#define _GNU_SOURCE          /* for some libc prototypes */
#endif
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*--------------------- prime sieve -----------------------------------*/
#define PRIME_LIMIT 2000000               /* enough for all factors up to 18m+1 (m moderately large) */
static uint8_t sieve[PRIME_LIMIT + 1];
static uint64_t primes[160000];
static int prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt != 0)
        return;                           /* already initialized */

    /* simple sieve of Eratosthenes */
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        sieve[i] = 1;                      /* assume prime */

    for (int i = 2; (long long)i * i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 0;

    prime_cnt = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            primes[prime_cnt++] = i;
}

/*--------------------- factorisation ---------------------------------*/
static bool factor_squarefree(uint64_t n, uint64_t *out, int *cnt)
{
    int c = 0;
    uint64_t temp = n;
    for (int i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            int mult = 0;
            while (temp % p == 0) {
                temp /= p;
                ++mult;
                if (mult > 1)          /* repeated prime factor */
                    return false;
            }
            out[c++] = p;
        }
    }
    if (temp > 1)                      /* remaining factor is prime */
        out[c++] = temp;
    *cnt = c;
    return true;
}

/*--------------------- Carmichael test -------------------------------*/
static bool good_m(uint64_t m)
{
    uint64_t a = 6ULL * m + 1;
    uint64_t b = 12ULL * m + 1;
    uint64_t c = 18ULL * m + 1;

    uint64_t f[20];
    int cnt, all[60], all_cnt = 0;

    /* factor a */
    if (!factor_squarefree(a, f, &cnt))
        return false;
    for (int i = 0; i < cnt; ++i) {
        uint64_t p = f[i];
        for (int j = 0; j < all_cnt; ++j)
            if (all[j] == p)
                return false;           /* duplicate prime across factors */
        all[all_cnt++] = p;
    }

    /* factor b */
    if (!factor_squarefree(b, f, &cnt))
        return false;
    for (int i = 0; i < cnt; ++i) {
        uint64_t p = f[i];
        for (int j = 0; j < all_cnt; ++j)
            if (all[j] == p)
                return false;
        all[all_cnt++] = p;
    }

    /* factor c */
    if (!factor_squarefree(c, f, &cnt))
        return false;
    for (int i = 0; i < cnt; ++i) {
        uint64_t p = f[i];
        for (int j = 0; j < all_cnt; ++j)
            if (all[j] == p)
                return false;
        all[all_cnt++] = p;
    }

    /* Korselt's criterion: for each prime p dividing N, (p-1) | (N-1) */
    for (int i = 0; i < all_cnt; ++i) {
        uint64_t p = all[i];
        uint64_t mod = p - 1;
        if (mod == 0)
            return false;
        uint64_t rem = a % mod;
        rem = (uint64_t)((__int128)rem * (b % mod)) % mod;
        rem = (uint64_t)((__int128)rem * (c % mod)) % mod;
        if (rem != 1 % mod)
            return false;
    }
    return true;
}

/*--------------------- cache for sequence values ---------------------*/
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static uint64_t last_checked_m = 0;

/*--------------------- main function ---------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for offset 1 */

    init_primes();

    if (n <= cache_len)
        return cache[n - 1];

    /* enlarge cache as needed */
    if (cache_cap == 0) {
        cache_cap = 256;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }
    while (cache_len < n) {
        ++last_checked_m;
        if (good_m(last_checked_m)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = (seq_t)last_checked_m;
        }
    }
    return cache[n - 1];
}
