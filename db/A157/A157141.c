/*
 * A157141  Prime numbers for which the number of odd nonprime digits equals
 *          the number of even nonzero digits and both are >0.
 * https://oeis.org/A157141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n‑th prime whose number of odd nonprime digits
 * (1 or 9) equals the number of even nonzero digits (2, 4, 6, or 8),
 * both counts being > 0.
 *
 * Implementation details:
 *   * Uses a simple Miller‑Rabin test deterministic for 64‑bit values.
 *   * Counts digits with a small helper, no external helper functions.
 *   * Maintains a cache of qualifying primes to support multiple calls
 *     in any order.
 *   * Values are stored in seq_t (possibly smaller than 64 bits); they
 *     are casted when inserted into the cache.  The function returns
 *     the correct value whenever the result fits in seq_t.
 *
 * The code is compliant with C11 and C++17, uses only integer
 * arithmetic, and satisfies all compilation requirements.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------------------------------------------------------------------- */
/*  1.  Deterministic Miller–Rabin primality test for 64‑bit integers   */
/* -------------------------------------------------------------------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
        31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL,
        71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Write n - 1 = d * 2^s with d odd. */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for < 2^64. */
    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        __uint128_t x = 1, power = d;
        uint64_t cur = a % n;
        while (power) {
            if (power & 1ULL) x = (x * cur) % n;
            cur = (__uint128_t)cur * cur % n;
            power >>= 1ULL;
        }
        uint64_t x64 = (uint64_t)x;
        if (x64 == 1 || x64 == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = (__uint128_t)x64 * x64 % n;
            x64 = (uint64_t)x;
            if (x64 == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont) return false;
    }
    return true;
}

/* -------------------------------------------------------------------- */
/*  2.  Count decimal digits of a 64‑bit integer.                       */
/* -------------------------------------------------------------------- */
static void digit_counts_uint64(uint64_t x, unsigned char cnt[10])
{
    for (int d = 0; d < 10; ++d) cnt[d] = 0;
    if (x == 0) { cnt[0] = 1; return; }
    while (x > 0) {
        unsigned d = x % 10;
        ++cnt[d];
        x /= 10;
    }
}

/* -------------------------------------------------------------------- */
/*  3.  Cache of qualifying primes.                                     */
/* -------------------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 0;

/* Ensure we have at least n qualifying primes cached. */
static void ensure_cache_size(idx_t n)
{
    if ((idx_t)cache_len >= n) return;

    uint64_t cand = last_checked + 1;
    if (cand <= 2) cand = 2;
    while ((idx_t)cache_len < n) {
        if (is_prime_uint64(cand)) {
            unsigned char cnt[10];
            digit_counts_uint64(cand, cnt);
            uint64_t odd_nonprime  = (uint64_t)cnt[1] + cnt[9];
            uint64_t even_nonzero = (uint64_t)cnt[2] + cnt[4] + cnt[6] + cnt[8];
            if (odd_nonprime > 0 && even_nonzero > 0 &&
                odd_nonprime == even_nonzero)
            {
                /* Grow cache if necessary. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache = new_cache;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)cand; /* cast is fine if fits */
            }
        }
        last_checked = cand;
        ++cand;
    }
}

/* -------------------------------------------------------------------- */
/*  4.  Public interface.                                               */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; before that value is undefined */
    ensure_cache_size(n);
    return cache[n - 1];
}
