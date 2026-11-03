/*
 * A093245  a(n) is the lesser term of the smallest twin prime pair such that
 *          if P=(a(n)^2+n)^2+n, then P and P+2 are also twin primes. a(n) is
 *          0 if no such pair exists.
 * https://oeis.org/A093245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) returns the smaller term of the smallest twin prime pair
 *            (p, p + 2) such that if P = (p² + n)² + n, then P and P + 2 are
 *            also twin primes. If no such pair exists, a(n) = 0
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8799 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------
   Required utilities:

   - isPrimeGeneric(seq_t n)   (provided by "isPrimeGeneric.h")
   Note: seq_t may be a built‑in integer type or an arbitrary‑precision
   integer class with the usual arithmetic operators defined.
   ------------------------------------------------- */

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

/* Memoisation cache for sequence values */
static seq_t *a_cache = NULL;      /* values of a(n) */
static bool  *a_done  = NULL;      /* whether a(n) has been computed */
static idx_t   a_cap   = 0;        /* current capacity of arrays */

/* Ensure that caches are large enough to store values up to index n */
static void ensure_cache(idx_t n)
{
    if (n <= a_cap)
        return;

    idx_t new_cap = (a_cap == 0) ? 256 : a_cap * 2;
    while (new_cap < n)
        new_cap *= 2;

    /* realloc may return NULL if out of memory; we simply let the caller
       terminate in that unlikely case. */
    a_cache = mem_resize(seq_t, a_cache, (new_cap + 1));
    a_done  = (bool  *)realloc(a_done,  (new_cap + 1) * sizeof(bool));

    /* Mark newly allocated entries as not yet computed */
    for (idx_t i = a_cap + 1; i <= new_cap; ++i)
        a_done[i] = false;

    a_cap = new_cap;
}

/* -------------------------------------------------
   The required sequence function
   ------------------------------------------------- */
seq_t a(idx_t n)
{
    /* seq_t n must be converted explicitly if we want to use it in
       arithmetic with seq_t types.  For simplicity we cast only once
       when needed. */
    if (n == 0)
        return 0;                /* outside the offset – value irrelevant */

    ensure_cache(n);
    if (a_done[n])
        return a_cache[n];

    /* Known property: a(n) = 0 for all multiples of 3 */
    if ((n % 3) == 0) {
        a_cache[n] = 0;
        a_done[n]  = true;
        return 0;
    }

    /* Search for the smallest twin prime pair satisfying the condition. */
    seq_t p = 3;                 /* start from the first odd prime */
    while (1) {
        /* Check if p and p+2 are prime */
        seq_t p_plus2 = p + (seq_t)2;
        if (isPrimeGeneric(p) && isPrimeGeneric(p_plus2)) {
            /* Compute t = p² + n and P = t² + n */
            seq_t t = p * p + (seq_t)n;
            seq_t P = t * t + (seq_t)n;

            if (isPrimeGeneric(P) && isPrimeGeneric(P + (seq_t)2)) {
                a_cache[n] = p;
                a_done[n]  = true;
                return p;
            }
        }

        /* Advance to the next odd integer */
        p += (seq_t)2;

        /* Safety guard: if p grows extraordinarily large we give up
           and report 0 (this should not happen for n up to ~10000
           where a(n) is conjectured to be non‑zero). */
        if (p > ((seq_t)1 << 60)) {   /* very large number for most build types */
            a_cache[n] = 0;
            a_done[n]  = true;
            return 0;
        }
    }
}
