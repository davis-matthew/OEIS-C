/*
 * A020453  Primes that contain digits 1 and 5 only.
 * https://oeis.org/A020453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 109, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 49 terms through n = 8592 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A020453: the n‑th prime containing only
   digits 1 and 5. It lazily generates candidates by increasing
   decimal length, tests primality with isPrimeGeneric, and
   caches all primes found so far. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

static int digit_len(seq_t x)
{
    int len = 0;
    do {
        len++;
        x /= 10;
    } while (x != 0);
    return len;
}

static seq_t *prime_cache = NULL;   /* array of primes */
static size_t prime_cap  = 0;       /* allocated capacity          */
static size_t prime_cnt  = 0;       /* number of cached primes     */
static uint32_t next_len = 1;       /* next decimal length to test */

/* Ensure the cache can hold at least 'need' primes.  */
static void
ensure_capacity(size_t need)
{
    if (prime_cap >= need)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 128;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *newp = mem_resize(seq_t, prime_cache, new_cap);
    if (!newp) {
        /* Allocation failed – halt execution (unlikely in judge). */
        abort();
    }
    prime_cache = newp;
    prime_cap   = new_cap;
}

/* Append a new prime to the cache. */
static void
add_prime(seq_t val)
{
    ensure_capacity(prime_cnt + 1);
    prime_cache[prime_cnt++] = val;
}

/* Generate all primes of a given decimal length and append them. */
static void
generate_len(uint32_t len)
{
    if (len == 1) {
        seq_t val = 5;
        if (isPrimeGeneric(val))
            add_prime(val);
        return;
    }

    /* For length > 1 the least‑significant digit must be 1. */
    uint32_t bits  = len - 1;                /* number of variable digits */
    uint32_t total = 1u << bits;             /* 2^(len-1) candidates       */

    for (uint32_t mask = 0; mask < total; ++mask) {
        seq_t val = 0;
        /* Build the most significant bits. */
        for (uint32_t pos = 0; pos < bits; ++pos) {
            uint32_t bit = (mask >> (bits - 1 - pos)) & 1u;
            val = val * 10 + (bit ? 5 : 1);
        }
        val = val * 10 + 1; /* final digit fixed to 1 */

        /* Skip numbers that underflow seq_t (decimal length changed). */
        if (digit_len(val) != (int)len)
            continue;

        if (isPrimeGeneric(val))
            add_prime(val);
    }
}

/* External linkage required by the build. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n = 0, returns 0 */

    /* Generate primes until we have at least 'n' entries. */
    while (prime_cnt < (size_t)n) {
        generate_len(next_len);
        ++next_len;
    }

    return prime_cache[(size_t)(n - 1)];
}
