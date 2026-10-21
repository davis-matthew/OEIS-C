/*
 * A087090  Positive numbers n such that p=n^2+n+41 and p+2 are twin primes.
 * https://oeis.org/A087090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1468, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n‑th integer n such that  n^2 + n + 41  and  n^2 + n + 43  are primes.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/append_candidate.h"

/* Deterministic Miller–Rabin for 64‑bit integers.  */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)                  return false;
    if (n % 2 == 0)             return n == 2;
    if (n % 3 == 0)             return n == 3;

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Bases that guarantee correctness for all 64‑bit values. */
    const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                              450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        __uint128_t x = 1, base = a, exp = d;
        /* fast power modulo n */
        while (exp) {
            if (exp & 1) x = (x * base) % n;
            base = (base * base) % n;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Static cache of discovered n‑values. */
static seq_t *cache_arr = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_try = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        uint64_t cand = (uint64_t)next_try;
        /* p = n^2 + n + 41; p+2 = n^2 + n + 43 */
        __uint128_t val128 = (__uint128_t)cand * cand + cand + 41;
        uint64_t p = (uint64_t)val128;
        if (is_prime_uint64(p) && is_prime_uint64(p + 2)) {
            append_candidate(&cache_arr, &cache_len, &cache_cap,
                             (seq_t)cand);
        }
        ++next_try;
    }
    return cache_arr[(size_t)n - 1];
}
