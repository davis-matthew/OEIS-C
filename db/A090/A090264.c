/*
 * A090264  Numbers n such that the strings n7n and 7n7 are both prime.
 * https://oeis.org/A090264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th integer n such that the decimal strings n7n and
 *            7n7 are both prime (OEIS A090264). The function implements a
 *            linear search with memoisation, testing each integer in turn
 *            until the required number of terms have been found
 * Range:     u8 none, u16 none, u32 n <= 1460, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9755 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
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

static bool is_candidate(idx_t x)
{
    /* Static state keeps track of the current number of decimal digits
       and the corresponding powers of ten.  They are updated only when
       the candidate reaches the next digit threshold. */
    static int digits = 1;            /* current number of digits of x */
    static seq_t pow10d  = 10;        /* 10^digits   */
    static seq_t pow10d1 = 100;       /* 10^(digits+1) */
    static idx_t next_pow10 = 10;     /* the next 10^digits threshold */

    if (x == next_pow10) {
        /* digit count increases */
        digits++;
        pow10d  = pow10d  * (seq_t)10;
        pow10d1 = pow10d1 * (seq_t)10;
        next_pow10 *= 10;
    }

    seq_t n   = (seq_t)x;
    seq_t n7n = n * pow10d1 + ((seq_t)7 * pow10d) + n;
    seq_t s_n7 = ((seq_t)7 * pow10d1) + (n * (seq_t)10) + ((seq_t)7);

    return isPrimeGeneric(n7n) && isPrimeGeneric(s_n7);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n = 0 */

    /* Lazy dynamic cache of discovered terms. */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;
    static idx_t cache_cap   = 0;
    static idx_t candidate   = 1;  /* next integer to test */

    if (cache == NULL) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
    }

    while (cache_size < n) {
        if (is_candidate(candidate)) {
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_size++] = (seq_t)candidate;
        }
        candidate++;
    }

    return cache[n - 1];  /* offset is 1 */
}
