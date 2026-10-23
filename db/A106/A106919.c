/*
 * A106919  Primes of the form 3x^2+xy+5y^2, with x and y any integer.
 * https://oeis.org/A106919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 2198, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A106919:
   a(n) = the n-th prime of the form 3x^2 + x y + 5 y^2   (x, y ∈ ℤ)  */
/* Note: the value is returned as seq_t, with the typical cast
   (seq_t)uint64_t value. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* ---------- integer sqrt of a 128‑bit value ---------- */
static uint64_t isqrt_u128(__uint128_t n)
{
    /* floor(sqrt(n)).  The result is < 2^35 for n <= 12*2^64. */
    uint64_t low = 0;
    uint64_t high = (uint64_t)1 << 35;  /* > sqrt(12*2^64) ≈ 1.48e10 */
    while (low < high) {
        uint64_t mid = (low + high) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* ---------- check whether a prime can be written as 3x^2+xy+5y^2 ---------- */
static bool has_representation(uint64_t p)
{
    /* compute maximal |y|: y^2 <= 12p/59  ->  y <= sqrt(12p/59) */
    __uint128_t tmp = (__uint128_t)12 * p;
    uint64_t ymax = isqrt_u128(tmp / 59);

    for (int64_t y = -(int64_t)ymax; y <= (int64_t)ymax; ++y) {
        __uint128_t yy = (__uint128_t)(y >= 0 ? y : -y) * (uint64_t)(y >= 0 ? y : -y);
        __uint128_t D = (__uint128_t)12 * p - (__uint128_t)59 * yy;
        if (D > ( __uint128_t)UINT64_MAX * ( __uint128_t)UINT64_MAX) continue; /* overflow guard */
        if (D > (__uint128_t)UINT64_MAX * (__uint128_t)UINT64_MAX) continue;
        uint64_t sqrtD = isqrt_u128(D);
        if ((__uint128_t)sqrtD * sqrtD != D)
            continue;
        /* Check x integer condition: (-y ± sqrtD) divisible by 6 */
        int64_t num1 = -(int64_t)y + (int64_t)sqrtD;
        int64_t num2 = -(int64_t)y - (int64_t)sqrtD;
        if (num1 % 6 == 0 || num2 % 6 == 0)
            return true;
    }
    return false;
}

/* ---------- cache of discovered terms ---------- */
static uint64_t *prime_cache = NULL;
static size_t prime_cnt  = 0;  /* number of cached terms */
static size_t prime_cap  = 0;  /* capacity of prime_cache */
static uint64_t next_candidate = 2;  /* next integer to test */

/* ---------- ensure prime_cache has at least 'need' capacity ---------- */
static void ensure_cache_cap(size_t need)
{
    if (prime_cap >= need)
        return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 8;
    if (new_cap < need)
        new_cap = need;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp) abort();  /* out of memory */
    prime_cache = tmp;
    prime_cap = new_cap;
}

/* ---------- add a new term to the cache ---------- */
static void push_prime(uint64_t val)
{
    ensure_cache_cap(prime_cnt + 1);
    prime_cache[prime_cnt++] = val;
}

/* ---------- build cache up to n terms ---------- */
static void build_to(idx_t n)
{
    while (prime_cnt < n) {
        /* advance to the next prime candidate */
        while (!is_prime(next_candidate))
            ++next_candidate;

        if (has_representation(next_candidate))
            push_prime(next_candidate);

        ++next_candidate;
    }
}

/* ---------- required function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for offset 0, behaviour unspecified */
    build_to(n);
    return (seq_t)prime_cache[n - 1];
}
