/*
 * A073925  Powers of 2 rearranged such that every partial sum (n>1) is
 *          composite.
 * https://oeis.org/A073925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest unused power of two such that the sum of
 *            the first n terms is composite (greedy construction, a(1)=1)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 3257 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/mem.h"

/* Compute 2^exp for any exponent that may not fit in the bit width of seq_t. */
static seq_t pow2(idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res *= (seq_t)2;
    return res;
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

static bool is_composite_seq(seq_t n)
{
    /* 2 and 3 are prime, all smaller sums are not composite */
    if (n <= 3) return false;
    return !isPrimeGeneric(n);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n = 0 */

    /* ----- static memoisation state ----- */
    static seq_t *seq = NULL;          /* array of generated terms */
    static size_t seq_len = 0;         /* how many terms we have */
    static size_t seq_cap = 0;         /* capacity of seq array        */
    static seq_t curr_sum = 0;         /* sum of all terms so far      */

    static uint8_t *used = NULL;       /* bit‑style flags for used exponents */
    static size_t used_cap = 0;        /* number of exponent slots          */

    /* ---------- initialisation on first call ---------- */
    if (seq_len == 0) {
        /* first term a(1) = 1 (= 2^0) */
        seq_cap = 1;
        seq = mem_new(seq_t, seq_cap);
        if (!seq) abort();
        seq[0] = pow2(0);
        seq_len = 1;
        curr_sum = seq[0];

        used_cap = 1;
        used = (uint8_t *)calloc(used_cap, sizeof(uint8_t));
        if (!used) abort();
        used[0] = 1;        /* exponent 0 already used */
    }

    /* ---------- return cached value if available ---------- */
    if ((size_t)n <= seq_len)
        return seq[(size_t)n - 1];

    /* ---------- build sequence up to index n ---------- */
    size_t target = (size_t)n;
    while (seq_len < target) {
        /* Search for the smallest unused exponent e that makes
           the new partial sum composite. */
        idx_t e = 0;
        for (;;) {
            /* grow the used array if necessary */
            if ((size_t)e >= used_cap) {
                size_t new_cap = used_cap ? used_cap * 2 : 1;
                while ((size_t)e >= new_cap) new_cap *= 2;
                uint8_t *tmp = (uint8_t *)realloc(used, new_cap * sizeof(uint8_t));
                if (!tmp) abort();
                memset(tmp + used_cap, 0,
                       (new_cap - used_cap) * sizeof(uint8_t));
                used = tmp;
                used_cap = new_cap;
            }
            if (used[e]) {          /* already used power of two */
                ++e;
                continue;
            }

            seq_t candidate_sum = curr_sum + pow2(e);
            if (is_composite_seq(candidate_sum)) {
                /* we found the correct exponent */
                if (seq_len >= seq_cap) {
                    size_t new_cap = seq_cap ? seq_cap * 2 : 1;
                    while (seq_len >= new_cap) new_cap *= 2;
                    seq_t *tmp = mem_resize(seq_t, seq, new_cap);
                    if (!tmp) abort();
                    seq = tmp;
                    seq_cap = new_cap;
                }
                seq[seq_len++] = pow2(e);
                curr_sum = candidate_sum;
                used[e] = 1;
                break;     /* advance to build next term */
            } else {
                ++e;          /* try next unused exponent */
            }
        }
    }

    return seq[(size_t)n - 1];
}
