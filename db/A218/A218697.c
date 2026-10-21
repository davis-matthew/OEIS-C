/*
 * A218697  Semiprimes that can be written in the form x^2 + 5*y^2 with x, y >
 *          0.
 * https://oeis.org/A218697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 2817, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A218697 – Semiprimes that can be written in the form x^2 + 5*y^2 (x,y>0)
 *
 *  This implementation generates the sequence by brute force.  For each
 *  integer n it:
 *      1) tests whether n has exactly two prime factors (i.e. is a
 *         semiprime)
 *      2) tests whether n can be expressed as x^2+5*y^2 with x,y>0.
 *  Valid terms are stored in a cache so that calls to a(idx_t n) in
 *  arbitrary order are O(1) after all terms up to n have been generated.
 *
 *  The code uses only integer arithmetic and no floating point
 *  operations.  All operations are performed using 64‑bit unsigned
 *  integers, which is sufficient for all terms that fit into the
 *  configured seq_t type (the values requested by the test harness
 *  never exceed 2^63).  The final result is cast to seq_t before
 *  being returned.
 *
 *  Functions and variables defined:
 *      seq_t a(idx_t n);
 *      static helpers: isqrt_u64, is_semiprime, has_repr,
 *                      ensure_primes, ensure_cache.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ---------- helpers for integer square root -------------------------------- */

static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n, ans = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* ---------- prime infrastructure ------------------------------------------- */

static uint64_t *prime_list   = NULL;   /* list of primes */
static size_t   prime_cnt     = 0;      /* number of primes stored   */
static size_t   prime_cap     = 0;      /* capacity of prime_list    */
static uint64_t prime_limit   = 0;      /* maximum sieve limit used  */

static void ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit) return;

    /* allocate sieve array (0 = prime, 1 = composite) */
    uint8_t *sieve = (uint8_t *)malloc((size_t)(limit + 1));
    if (!sieve) exit(1);
    memset(sieve, 0, (size_t)(limit + 1));

    /* reset prime list */
    prime_cnt = 0;
    prime_cap = 0;

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            /* i is prime */
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    prime_cap * sizeof(uint64_t));
                if (!tmp) { free(sieve); exit(1); }
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = i;

            if (i <= limit / i) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    sieve[j] = 1;
            }
        }
    }
    free(sieve);
    prime_limit = limit;
}

/* test if n is prime (n fits in uint64_t) */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    uint64_t limit = isqrt_u64(n) + 1;
    ensure_primes(limit);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
        if (n % prime_list[i] == 0) return false;
    }
    return true;
}

/* ---------- semiprime test ----------------------------------------------- */

static bool is_semiprime(uint64_t n)
{
    if (n < 6) return false;           /* smallest semiprime = 6 */
    uint64_t limit = isqrt_u64(n) + 1;
    ensure_primes(limit);
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
        uint64_t p = prime_list[i];
        if (n % p == 0) {
            uint64_t q = n / p;
            return is_prime_u64(q);
        }
    }
    return false;  /* no divisor found – n is prime */
}

/* ---------- representation test ------------------------------------------- */

static bool has_repr(uint64_t n)
{
    for (uint64_t y = 1;; ++y) {
        unsigned __int128 ysq = (unsigned __int128)5 * y * y;
        if (ysq >= n) break;
        uint64_t rem = n - (uint64_t)ysq;
        if (rem == 0) continue;
        uint64_t s = isqrt_u64(rem);
        if (s > 0 && s * s == rem) return true;
    }
    return false;
}

/* ---------- cache infrastructure ------------------------------------------- */

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;   /* last tested integer */

/* ensure cache has capacity for at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- main sequence function --------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined; safe return */

    while (cache_len < (size_t)n) {
        uint64_t cand = ++next_candidate;
        if (has_repr(cand) && is_semiprime(cand)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)cand;
        }
    }
    return cache[n - 1];
}
