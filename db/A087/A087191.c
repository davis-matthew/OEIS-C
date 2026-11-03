/*
 * A087191  Primes of the form k^4 - (k+1)^3.
 * https://oeis.org/A087191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 61, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 44 terms through n = 4874 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A087191: primes of the form k^4 – (k+1)^3
   Offset 1 – a(1) corresponds to k = 3.  Each term is tested for
   primality before being cached so that calls for arbitrary n
   (in any order) are answered in O(1) after the first expansion. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/mem.h"

static bool isPrime128(__uint128_t n)
{
    if (n < 2) return false;

    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Deterministic bases for 128‑bit integers (covers < 2^128) */
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                                     19ULL,23ULL,29ULL,31ULL,37ULL};

    uint64_t d = (uint64_t)(n - 1);
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        __uint128_t x = 1, pw = d, base = a;
        while (pw) {
            if (pw & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            pw >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* -----------------  Helper:  Miller–Rabin for 64‑bit ----------------- */
static bool isPrimeULL(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0) return n == 2ULL;

    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL,
                                     28178ULL, 450775ULL, 9780504ULL,
                                     1795265022ULL};
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, pw = d, base = a;
        while (pw) {
            if (pw & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            pw >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return isPrimeULL((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return isPrime128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    /* cache of found primes, growing as needed */
    static seq_t *cache   = NULL;
    static idx_t  len     = 0;   /* number of primes stored   */
    static idx_t  cap     = 0;   /* current allocation size   */
    static idx_t  next_k  = 3;   /* next k value to try        */

    /* expand the cache if we need more space */
    if (n > cap) {
        idx_t new_cap = cap ? cap << 1 : 8;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        cache = tmp;
        cap   = new_cap;
    }

    /* generate primes until we have at least n of them */
    while (len < n) {
        seq_t val;
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            /* 64‑bit seq_t – use 128‑bit intermediates to avoid overflow */
            unsigned __int128 k  = (unsigned __int128)next_k;
            unsigned __int128 k4 = k * k * k * k;
            unsigned __int128 kp1= k + 1;
            unsigned __int128 kp13= kp1 * kp1 * kp1;
            unsigned __int128 val128 = k4 - kp13;

            if (val128 > (unsigned __int128)UINT64_MAX) {
                /* we cannot represent larger values in a 64‑bit type;
                   subsequent terms would also be too large, so stop. */
                break;
            }
            val = (seq_t)val128;
        } else {
            /* seq_t is larger than 64‑bit – use seq_t arithmetic
               which is defined for the arbitrary‑precision type. */
            seq_t k   = (seq_t)next_k;
            seq_t k4  = k * k * k * k;
            seq_t kp1 = k + 1;
            seq_t kp13= kp1 * kp1 * kp1;
            val = k4 - kp13;
        }

        if (isPrimeGeneric(val)) {
            cache[len++] = val;
        }
        ++next_k;
    }

    return cache[n - 1];
}
