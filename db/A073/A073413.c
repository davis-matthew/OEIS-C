/*
 * A073413  Numbers n such that number of divisors of n divides S(n), the
 *          Kempner function A002034.
 * https://oeis.org/A073413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   legendre_uint64, min_m_for_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: d(n) divides Kempner function S(n) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/legendre_uint64.h"
#include "../utils/min_m_for_prime_power.h"

/* static storage for primes */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

/* cache for sequence terms */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* number currently being tested */
static uint64_t current_num = 1;

/* integer square root of uint64_t, returns floor(sqrt(n)) */
static uint32_t isqrt_uint64(uint64_t n)
{
    uint64_t bit = 1ULL << 62;
    uint64_t r   = 0;
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return (uint32_t)r;
}

/* ensure that primes list contains all primes <= limit */
static void ensure_primes_up_to(uint32_t limit)
{
    if (primes == NULL) {
        primes_cnt = 0;
        primes_cap = 16;
        primes = (uint32_t *)calloc(primes_cap, sizeof(uint32_t));
    }
    if (primes_cnt > 0 && limit <= primes[primes_cnt - 1]) return;
    uint32_t start = (primes_cnt == 0) ? 2 : primes[primes_cnt - 1] + 1;
    for (uint32_t cand = start; cand <= limit; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= cand; ++i) {
            if (cand % primes[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = cand;
        }
    }
}

/* check whether n satisfies d(n) | S(n) */
static bool satisfies_property(uint64_t n)
{
    uint64_t tmp = n;
    uint64_t d   = 1;
    uint64_t S   = 0;

    uint32_t limit = isqrt_uint64(tmp);
    ensure_primes_up_to(limit);

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            uint32_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            d *= (uint64_t)(e + 1);
            uint64_t mp = min_m_for_prime_power(p, e);
            if (mp > S) S = mp;
        }
    }
    if (tmp > 1) {
        d *= 2;
        uint64_t mp = min_m_for_prime_power((uint32_t)tmp, 1);
        if (mp > S) S = mp;
    }
    return (S % d) == 0;
}

/* ensure terms array capacity */
static void ensure_terms_capacity(size_t min_cap)
{
    if (terms_cap >= min_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* public function a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (terms_len < (size_t)n) {
        ensure_terms_capacity(n);
        while (terms_len < (size_t)n) {
            if (satisfies_property(current_num))
                terms[terms_len++] = (seq_t)current_num;
            ++current_num;
        }
    }
    return terms[n - 1];
}
