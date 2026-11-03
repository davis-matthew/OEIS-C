/*
 * A109306  Numbers k such that k^2 + (k-1)^2 and k^2 + (k+1)^2 are both
 *          primes.
 * https://oeis.org/A109306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 594, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 8694 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th positive integer k such that both
 * k² + (k−1)² and k² + (k+1)² are prime. 
 * The first term is 2. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Deterministic Miller‑Rabin for 64‑bit integers */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,
                                            13ULL,17ULL,19ULL,23ULL,29ULL,
                                            31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    /* Write n-1 = d * 2^s with d odd. */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* Bases sufficient for 64‑bit deterministic test. */
    const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL,
                              9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;            /* skip witnesses that equal 0 */
        __uint128_t x = 1, power = d;
        __uint128_t base = a;
        /* Compute a^d mod n using binary exponentiation. */
        while (power) {
            if (power & 1) x = (x * base) % n;
            base = (base * base) % n;
            power >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (cont) continue;
        return false;
    }
    return true;
}

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

static seq_t *cache = NULL;      /* dynamic array of seq_t terms */
static idx_t cache_len = 0;      /* number of terms stored */
static idx_t cache_cap = 0;      /* allocated capacity of cache */

/* Append a value to the cache using the helper push_seq. */
static void cache_push(seq_t val)
{
    push_seq(&cache, &cache_cap, &cache_len, val);
}

/* Return the n-th term of A109306.  (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset irrelevant for n < 1 */

    /* Ensure that the first term (k = 2) is present. */
    if (cache_len == 0)
        cache_push((seq_t)2);

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        /* Next candidate k starts after the last stored term */
        uint64_t k = (uint64_t)cache[cache_len - 1] + 1;

        for (;; ++k)
        {
            /* Compute val1 = k² + (k-1)²  = 2k² - 2k + 1
             * Compute val2 = k² + (k+1)²  = 2k² + 2k + 1
             * Use 128‑bit intermediate to avoid overflow.
             */
            unsigned __int128 sq = (unsigned __int128)k * k;
            uint64_t val1 = (uint64_t)(2 * sq - 2 * k + 1);
            uint64_t val2 = (uint64_t)(2 * sq + 2 * k + 1);

            if (is_prime_uint64(val1) && is_prime_uint64(val2))
            {
                cache_push((seq_t)k);
                break;   /* accept this k and continue */
            }
        }
    }

    return cache[n - 1];
}
