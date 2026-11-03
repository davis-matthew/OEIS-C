/*
 * A154676  Numbers n = 103*k^2 such that (n-1,n+1) is a twin prime pair (thus
 *          k = 6*m).
 * https://oeis.org/A154676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   * a(n) = 3708 * m², * where m is the n‑th positive integer such
 *            that 3708*m² − 1 and * 3708*m² + 1 are both prime. (m itself is
 *            not prime in general.)
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 9820 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
 
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
 
/* ------------------------------------------------------------------ */
/*  Dynamic cache of computed terms.  The cache grows as needed.        */
static seq_t *cache     = NULL;   /* array of terms */
static idx_t  cache_sz  = 0;      /* number of terms stored       */
static size_t cache_cap = 0;      /* allocated capacity            */
static idx_t  cur_m     = 0;      /* last candidate m examined    */
 
/* ------------------------------------------------------------------ */
/*  Helper to grow the cache when necessary. */
static void ensure_cache_capacity(void)
{
    if (cache_sz == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 1024 : cache_cap * 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) exit(EXIT_FAILURE);          /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }
}
 
/* ------------------------------------------------------------------ */
/*  Main sequence generator.  External linkage as required.           */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined offset 0 */
 
    /* Initialize cache on first use. */
    if (!cache) {
        cache = mem_new(seq_t, 1024);
        if (!cache) exit(EXIT_FAILURE);
        cache_cap = 1024;
    }
 
    /* Generate terms until we have at least n of them. */
    while (cache_sz < n) {
        cur_m++;                                 /* next candidate m  */
 
        /* Compute 3708 * m² in seq_t.  Cast m to seq_t before
           multiplication to avoid overflow of the 64‑bit intermediate. */
        seq_t val  = (seq_t)3708;
        val *= (seq_t)cur_m;
        val *= (seq_t)cur_m;                     /* val = 3708 * m²  */
 
        seq_t lo = val - 1;                      /* 3708*m² - 1      */
        seq_t hi = val + 1;                      /* 3708*m² + 1      */
 
        if (isPrimeGeneric(lo) && isPrimeGeneric(hi)) {
            ensure_cache_capacity();
            cache[cache_sz++] = val;             /* store the term   */
        }
    }
 
    return cache[n - 1];                         /* 1‑based indexing */
}
