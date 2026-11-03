/*
 * A087091  Numbers k such that p = k^2 + k + 41 and p - 2 are twin primes.
 * https://oeis.org/A087091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 2619, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9677 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find k such that k^2 + k + 41 and k^2 + k + 39 are both prime.
   Return the n‑th such k (1‑indexed).  This is the definition of
   OEIS A087091.  The sequence consists of the values of k. */

/* The build supplies the integer types seq_t and idx_t, along with
   the helper isPrimeGeneric(seq_t).  No other special facilities are
   required.  All arithmetic is performed on seq_t, which may be
   arbitrarily large. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

static seq_t *cache = NULL;      /* stored terms */
static size_t cache_len = 0;     /* number of terms currently stored */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t k_next = 0;         /* next k to test */

/* Expand the cache if it does not have the required capacity.  This
   routine is written in a way that is valid both in C and C++ (the
   cast from void* to seq_t* is required under C++). */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1024;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        abort();                /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th element of the sequence (1‑based).  If n == 0, the
   sequence is undefined; the function simply returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* defined only for n >= 1 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t k = (seq_t)k_next;                     /* current k */
        seq_t p = k * k + k + (seq_t)41;              /* p = k^2 + k + 41 */
        seq_t p_minus = p - (seq_t)2;                /* p-2 */

        if (isPrimeGeneric(p) && isPrimeGeneric(p_minus)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = k;
        }
        ++k_next;
    }
    return cache[n - 1];
}
