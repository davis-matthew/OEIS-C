/*
 * A057370  Numbers k that can be expressed as k = w+x = y*z with w*x =
 *          (y+z)^3 where w, x, y, and z are all positive integers.
 * https://oeis.org/A057370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers k such that ∃ w,x,y,z>0 with k = w + x = y*z and
 *            w*x = (y+z)^3. The function scans k increasing, tests all factor
 *            pairs (y,z) of k, and keeps the first n such k. An on‑demand
 *            sieve of primes and divisor enumeration are used for performance
 * Range:     u8 none, u16 n <= 29, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   isqrt_with_bound_signed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isqrt_with_bound_signed.h"

/* ---------- prime sieve (up to 100000) ---------- */
enum { PRIME_LIMIT = 100000, MAX_PRIMES = 9593 }; /* primes <= 100000 */
static uint32_t primes[MAX_PRIMES];
static size_t prime_count = 0;

static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    bool is_prime[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t p = 2; p * p <= PRIME_LIMIT; ++p)
        if (is_prime[p])
            for (size_t q = p * p; q <= PRIME_LIMIT; q += p)
                is_prime[q] = false;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) primes[prime_count++] = (uint32_t)i;
    done = true;
}

/* ---------- divisor enumeration ---------- */
enum { MAX_FACTORS = 16, MAX_DIVS = 16384 };
static uint64_t factor_p[MAX_FACTORS];
static int factor_e[MAX_FACTORS];
static uint64_t divs[MAX_DIVS];
static size_t divs_count;

/* factorize n into factor_p/factor_e (prime powers). */
static void factor(uint64_t n, size_t *cnt)
{
    size_t c = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            factor_p[c] = p;
            factor_e[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        factor_p[c] = n;
        factor_e[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* recursive generation of divisors. */
static void gen_divs_rec(int idx, size_t fcnt, uint64_t cur)
{
    if (idx == (int)fcnt) {
        if (divs_count < MAX_DIVS)
            divs[divs_count++] = cur;
        return;
    }
    uint64_t v = cur;
    for (int i = 0; i <= factor_e[idx]; ++i) {
        gen_divs_rec(idx + 1, fcnt, v);
        v *= factor_p[idx];
    }
}

/* ---------- check if k is a term of the sequence ---------- */
static bool good_k(uint64_t k)
{
    /* enumerate all factor pairs (y, z). */
    size_t fcnt;
    factor(k, &fcnt);
    divs_count = 0;
    gen_divs_rec(0, fcnt, 1);

    __int128 k128 = (__int128)k;
    for (size_t i = 0; i < divs_count; ++i) {
        uint64_t y = divs[i];
        uint64_t z = k / y;

        uint64_t s = y + z;
        __int128 s128 = (__int128)s;
        __int128 m128 = s128 * s128 * s128;      /* s^3 */
        __int128 D128 = k128 * k128 - (__int128)4 * m128;

        if (D128 < 0) continue;

        unsigned long long d = isqrt_with_bound_signed(D128, k);
        if ( (__int128)d * d != D128 ) continue;
        if ( ((k + d) & 1ULL) != 0 ) continue; /* parity check for integer w,x */
        return true;                           /* found a valid representation */
    }
    return false;
}

/* ---------- sequence storage + generation ---------- */
#define MAX_CACHE 20000
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;
static uint64_t checked_up_to = 0;

static void generate_up_to(size_t n)
{
    init_primes();
    while (cache_len < n) {
        ++checked_up_to;
        if (good_k(checked_up_to))
            cache[cache_len++] = (seq_t)checked_up_to;
    }
}

/* ----- required function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; value 0 undefined */
    if (cache_len < (size_t)n)
        generate_up_to(n);
    return cache[n-1];
}
