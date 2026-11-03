/*
 * A232268  Numbers n such that reversal (n^3) plus 1 is prime.
 * https://oeis.org/A232268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) is the nth positive integer n such that reverse(n³)+1 is
 *            prime
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 78, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9658 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <stddef.h>
#include <limits.h>

/* Reverse the decimal representation of a non‑negative integer stored in seq_t. */
static seq_t reverse_decimal(seq_t v)
{
    seq_t rev = 0;
    seq_t t   = v;
    while (t > 0) {
        rev = rev * 10 + (t % 10);
        t   = t / 10;
    }
    return rev;
}

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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Compute the n‑th term of sequence A232268.  The function is
   memoised so that calls for arbitrary n in any order are efficient. */
seq_t a(idx_t n)
{
    static seq_t *terms      = NULL;   /* cache of found terms (1‑based logic) */
    static size_t terms_len  = 0;      /* number of terms currently stored   */
    static size_t terms_cap  = 0;      /* allocated capacity of terms array   */
    static idx_t next_candidate = 1;   /* next integer to test              */

    /* Build enough terms to cover the requested index. */
    while (terms_len < (size_t)n) {
        idx_t m = next_candidate++;          /* candidate integer */

        /* Compute m³ in an idx_t to avoid overflow, then cast to seq_t. */
        idx_t cube_idx = m;
        cube_idx *= m;
        cube_idx *= m;
        seq_t cube = cube_idx;

        /* Reverse the cube and add 1. */
        seq_t rev = reverse_decimal(cube);
        seq_t candidate = rev + (seq_t)1;

        /* Test primality of candidate. */
        if (isPrimeGeneric(candidate)) {
            /* Ensure capacity for another term. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 1;
                seq_t *tmp = seq_realloc(terms, terms_cap, new_cap);
                if (!tmp) abort();           /* allocation failure */
                terms      = tmp;
                terms_cap  = new_cap;
            }
            terms[terms_len++] = m;           /* store the term */
        }
    }

    return terms[n - 1];
}
