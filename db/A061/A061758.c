/*
 * A061758  Positive integers k such that k! is divisible by (k+1)^10.
 * https://oeis.org/A061758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: integers k such that k! is divisible by (k+1)^10 (search). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ---------- Integer square root for idx_t ------------------- */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t left = 1, right = n / 2, mid, res = 0;
    while (left <= right) {
        mid = left + ((right - left) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            left = mid + 1;
            res = mid;
        } else {
            right = mid - 1;
        }
    }
    return res;
}

/* ---------- Dynamic prime list -------------------------------- */
static uint64_t *prime_list = NULL;      /* array of primes */
static size_t   prime_cnt  = 0;          /* number of primes stored */
static size_t   prime_cap  = 0;          /* allocated capacity */
static idx_t    prime_limit = 0;         /* largest number up to which primes are known */

/* Append a prime to the list, expanding storage if needed. */
static void append_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        prime_cap = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

/* Ensure all primes up to 'limit' are found (trial division). */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;
    idx_t start = prime_limit + 1;
    if (prime_limit == 0) start = 2;
    for (idx_t cand = start; cand <= limit; ++cand) {
        bool is_p = true;
        idx_t sq = isqrt_u64(cand);
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= sq; ++i) {
            if (cand % prime_list[i] == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p) append_prime(cand);
    }
    prime_limit = limit;
}

/* ---------- Test whether a single k satisfies the condition --- */
static bool k_satisfies(idx_t k)
{
    idx_t m   = k + 1;
    idx_t tmp = m;
    idx_t sqrt_m = isqrt_u64(m);
    ensure_primes_upto(sqrt_m);

    /* factor m and compare with exponent in k! */
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }

            /* Legendre's formula: exponent of p in k! */
            idx_t vp = 0;
            idx_t pk = p;
            while (pk <= k) {
                vp += k / pk;
                if (pk > k / p) break;  /* avoid overflow */
                pk *= p;
            }
            if (vp < 10 * e) return false;
        }
    }
    if (tmp > 1) {             /* remaining prime factor */
        uint64_t p = tmp;
        idx_t e = 1;
        idx_t vp = 0;
        idx_t pk = p;
        while (pk <= k) {
            vp += k / pk;
            if (pk > k / p) break;
            pk *= p;
        }
        if (vp < 10 * e) return false;
    }
    return true;
}

/* ---------- Cache for sequence values ----------------------- */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t   last_k = 0;   /* largest k already checked */

/* Extend the cache to contain at least 'target' terms. */
static void generate_until(size_t target)
{
    while (cache_len < target) {
        ++last_k;
        if (k_satisfies(last_k)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_k;
        }
    }
}

/* ---------- Public function --------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n < 1 */
    generate_until((size_t)n);
    return cache[n - 1];
}
