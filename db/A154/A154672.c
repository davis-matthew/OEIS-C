/*
 * A154672  Numbers n = 5*k^2 such that n - 1 and n + 1 are (twin) primes
 *          (thus k=6*m).
 * https://oeis.org/A154672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 215, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 44 terms through n = 9776 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A154672:
   a(n) = 180 * m² where m = n, and n−1, n+1 are both prime.  */
/* The sequence is obtained by generating the numbers 180*m²
   for integer m≥1 and collecting those for which the integers
   180*m²−1 and 180*m²+1 are twin primes.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

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

/* Static cache of already found terms. */
static seq_t *cache = NULL;      /* array of terms */
static size_t cache_len = 0;     /* number of cached terms */
static size_t cache_cap = 0;     /* allocated capacity */
static uint64_t last_m   = 0;    /* last m that was tested */

/* Ensure the cache can hold at least “needed” elements. */
static void
ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? (cache_cap << 1) : 8;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_cache = mem_new(seq_t, new_cap);
    if (!new_cache)
        abort();           /* memory exhaustion */

    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    mem_free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the nth element a(n) of the sequence (1‑based index). */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;   /* behaviour below offset does not matter */

    /* Generate terms until we have at least n stored. */
    while (cache_len < (size_t)n)
    {
        ++last_m;
        /* Compute 180 * m².  All arithmetic is performed in seq_t. */
        seq_t m_sq   = (seq_t)last_m * (seq_t)last_m;   /* m² */
        seq_t cand   = (seq_t)180 * m_sq;              /* 180*m² */

        /* Test twin primality of cand−1 and cand+1. */
        if (isPrimeGeneric(cand - (seq_t)1) &&
            isPrimeGeneric(cand + (seq_t)1))
        {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
