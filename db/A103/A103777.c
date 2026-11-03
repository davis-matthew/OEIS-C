/*
 * A103777  Numbers k such that f(k), f(k+1) and f(k+2) are all primes, where
 *          f(k) = 8*k^2 + 4*k + 1.
 * https://oeis.org/A103777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 131, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 974 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for the first n values of k such that
   f(k)=8*k^2+4*k+1, f(k+1), and f(k+2) are all prime.
   The results are cached to allow random‑order queries. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* primality test for 64‑bit unsigned integers */

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

/* ---- f(k) = 8*k^2 + 4*k + 1, computed in 128‑bit to avoid overflow ---- */
static uint64_t f_of(uint64_t k)
{
    __uint128_t v = (__uint128_t)k * k;          /* k² */
    v = v * 8 + (__uint128_t)4 * k + 1;         /* 8*k² + 4*k + 1 */
    return (uint64_t)v;
}

/* ---- resize helper for the cache array ---- */
static void ensure_capacity(size_t *cap, size_t needed, seq_t **arr)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap > 0) ? *cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();      /* terminate on out‑of‑memory */
    *arr = tmp;
    *cap = new_cap;
}

/* ---- main sequence generator ---- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0, sz = 0;
    static uint64_t last_k = 0;      /* last k that was checked */

    if (n == 0) return 0;            /* undefined, avoid UB */

    while (sz < n) {
        uint64_t k = last_k + 1;
        uint64_t f0 = f_of(k);
        uint64_t f1 = f_of(k + 1);
        uint64_t f2 = f_of(k + 2);

        if (is_prime_uint64(f0) && is_prime_uint64(f1) && is_prime_uint64(f2)) {
            ensure_capacity(&cap, sz + 1, &cache);
            cache[sz++] = (seq_t)k;
        }
        last_k = k;
    }

    return cache[n - 1];
}
