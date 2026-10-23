/*
 * A180929  Numbers whose sum of divisors is a pentagonal number.
 * https://oeis.org/A180929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 226, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A180929: Numbers whose sum of divisors is a pentagonal number.
   a(n) = the n-th positive integer m such that σ(m) is a pentagonal number.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_seq.h"

/* --------------------------------------------------------------------- */
/* Simple linear‑sieve up to 1,000,000 – enough for numbers up to 10⁴ */
static size_t primes_cnt = 0;
static uint32_t primes[78498];          /* primes <= 1,000,000 */
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;

    const uint32_t LIMIT = 1000000;
    bool *is_comp = (bool*)calloc(LIMIT + 1, sizeof(bool));
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                    is_comp[(size_t)j] = true;
            }
        }
    }
    free(is_comp);
    primes_initialized = true;
}

/* --------------------------------------------------------------------- */
/* Integer square‑root for 128‑bit values (binary search). */
static inline uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = (uint64_t)-1;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* --------------------------------------------------------------------- */
/* Test whether a 128‑bit integer is a pentagonal number. */
static inline bool is_pentagonal128(unsigned __int128 s)
{
    if (s == 0) return false;
    unsigned __int128 d = 1 + 24 * s;
    uint64_t sqrt_d = isqrt_u128(d);
    if ((unsigned __int128)sqrt_d * sqrt_d != d) return false;
    return ((uint64_t)(1 + sqrt_d) % 6ULL) == 0;
}

/* --------------------------------------------------------------------- */
/* Sum of divisors function σ(n) for 64‑bit n, returned as 128‑bit. */
static inline unsigned __int128 sigma_u128(uint64_t n)
{
    if (n == 1) return 1;
    init_primes();
    unsigned __int128 result = 1;
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            unsigned __int128 term = 1;
            unsigned __int128 power = 1;
            while (tmp % p == 0) {
                tmp /= p;
                power *= p;
                term += power;
            }
            result *= term;
        }
    }
    if (tmp > 1) {
        result *= ((unsigned __int128)1 + tmp);
    }
    return result;
}

/* --------------------------------------------------------------------- */
/* Sequence function – memoised, order‑independent. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour below is irrelevant */

    /* Static state: cache of found terms and next candidate to test. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t candidate = 1;

    /* Grow cache until it contains n terms. */
    while (cache_len < (size_t)n) {
        uint64_t m = candidate++;
        unsigned __int128 sigma = sigma_u128(m);
        if (is_pentagonal128(sigma)) {
            if (cache_len == cache_cap) {
                /* Grow by 1 (grow_seq expands as needed). */
                grow_seq(&cache, &cache_cap, cache_len + 1);
            }
            cache[cache_len++] = (seq_t)m;
        }
    }
    return cache[n - 1];
}
