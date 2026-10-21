/*
 * A057128  Numbers n such that -3 is a square mod n.
 * https://oeis.org/A057128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer n such that -3 is a quadratic
 *            residue modulo n. This is equivalent to requiring every prime
 *            divisor p>3 of n satisfy p≡1 (mod 6), no factor 2^k with k≥3,
 *            and no factor 3^k with k≥2
 * Range:     u8 n <= 80, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----- Utilities and headers ----- */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* ----- Small prime sieve for factorisation ----- */
#define SPF_LIMIT 2000000   /* upper bound for the smallest‑prime‑factor table */

static uint32_t *spf_arr = NULL;          /* smallest prime factor table */
static uint64_t *prime_list = NULL;       /* list of primes up to SPF_LIMIT */
static size_t prime_cnt = 0;              /* number of primes stored */

/* initialise spf_arr and prime_list once */
static void init_spf_and_primes(void)
{
    if (spf_arr != NULL)
        return;

    spf_arr = (uint32_t *)malloc((SPF_LIMIT + 1) * sizeof(uint32_t));
    if (!spf_arr) return;
    for (size_t i = 0; i <= SPF_LIMIT; ++i)
        spf_arr[(size_t)i] = (uint32_t)i;

    /* linear sieve for smallest prime factors */
    for (size_t i = 2; i * i <= SPF_LIMIT; ++i)
        if (spf_arr[i] == (uint32_t)i)
            for (size_t j = i * i; j <= SPF_LIMIT; j += i)
                if (spf_arr[j] == (uint32_t)j)
                    spf_arr[j] = (uint32_t)i;

    /* count primes and build list */
    for (size_t i = 2; i <= SPF_LIMIT; ++i)
        if (spf_arr[i] == (uint32_t)i)
            ++prime_cnt;
    prime_list = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!prime_list) return;
    size_t idx = 0;
    for (size_t i = 2; i <= SPF_LIMIT; ++i)
        if (spf_arr[i] == (uint32_t)i)
            prime_list[idx++] = (uint64_t)i;
}

/* return smallest prime factor of n (n may be > SPF_LIMIT) */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n <= SPF_LIMIT)
        return spf_arr[(size_t)n];
    uint64_t limit = (uint64_t)(sqrt((long double)n) + 1.0L);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
        uint64_t p = prime_list[i];
        if (n % p == 0)
            return p;
    }
    return n;   /* n is prime */
}

/* check whether n satisfies the sequence condition */
static bool ok_for_sequence(uint64_t n)
{
    if (n == 1)
        return true;

    uint64_t m = n;
    while (m > 1) {
        uint64_t p = smallest_prime_factor(m);
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if (p == 2) {
            if (exp >= 3) return false;   /* 2^3 = 8, 2^4 = 16, etc. fail */
        } else if (p == 3) {
            if (exp >= 2) return false;   /* 3^2 = 9 fails */
        } else {
            /* primes greater than 3 must be 1 (mod 6).  In mod 12 that is
               p ≡ 1 or 7. */
            if (p % 12 != 1 && p % 12 != 7)
                return false;
        }
    }
    return true;
}

/* ----- Cache for generated terms ----- */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* ensure cache holds at least 'n' terms */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_size)
        return;

    if (cache_size == 0) {
        /* initialise cache with first term 1 */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    uint64_t candidate = (cache_size > 0) ? (uint64_t)cache[cache_size - 1] + 1 : 2;
    while (cache_size < (size_t)n) {
        if (ok_for_sequence(candidate)) {
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)candidate;
        }
        ++candidate;
    }
}

/* ----- Required function ----- */
seq_t a(idx_t n)
{
    /* initialise helper data the first time */
    static bool init_done = false;
    if (!init_done) {
        init_spf_and_primes();
        init_done = true;
    }

    if (n == 0)
        return (seq_t)0;   /* undefined for offset 1, but return 0 */

    ensure_cache(n);
    return cache[n - 1];
}
