/*
 * A233906  Primes of the form (3^k mod k^3) + 1, in order of increasing k.
 * https://oeis.org/A233906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 65, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A233906.
   a(n) = the n-th prime of the form (3^k mod k^3) + 1,
   where k ≥ 2.  a(1) = 2. */

/* The definition requires a look‑ahead on k: we generate successive values
   of (3^k mod k^3) + 1 and keep the ones that are prime.  The cache keeps
   them in order so that calls to a(n) can arise in any order.  When the
   value is too large to fit in seq_t we stop and return 0.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <inttypes.h>

#include "../utils/mod_pow.h"      /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */

/* -------------------------------------------------------------
   64‑bit determinisitic Miller‑Rabin primality test
   ------------------------------------------------------------- */
static bool is_prime_u64(uint64_t n)
{
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
        73, 79, 83, 89, 97
    };
    size_t i;
    if (n < 2) return false;
    for (i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* Write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, pow = d;
        __uint128_t base = a;
        /* compute a^d mod n */
        while (pow) {
            if (pow & 1) x = (x * base) % n;
            base = (base * base) % n;
            pow >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        int r;
        for (r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) break;
        }
        if (r == s) return false;
    }
    return true;
}

/* Use is_prime_u64 when seq_t fits in 64 bits, otherwise fallback to the
   generic big‐integer routine. */
static bool is_prime_seq(seq_t n)
{
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        return is_prime_u64((uint64_t)n);
    } else {
        return is_prime_big(n);
    }
}

/* -------------------------------------------------------------
   Main sequence function
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache holds the discovered primes */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* next k to test (starting from 2 → a(1)=2) */
    static idx_t next_k = 2;

    if (n == 0) return (seq_t)0;  /* offset is 1 */

    /* grow cache until we have at least n terms or overflow occurs */
    while (cache_len < n) {
        /* compute k^3, checking for overflow */
        seq_t k3;
        bool overflow = false;

        if (sizeof(seq_t) <= 8) {          /* 64‑bit or smaller */
            __uint128_t k = (__uint128_t)next_k;
            __uint128_t c = k * k * k;
            if (c > (__uint128_t)UINT64_MAX) {
                overflow = true;
            } else {
                k3 = (seq_t)c;
            }
        } else {                          /* 128‑bit or big integer */
            seq_t kk = (seq_t)next_k;
            seq_t k2 = kk * kk;
            k3 = k2 * kk;
        }

        if (overflow) break;        /* cannot represent further values */

        /* compute val = (3^k mod k^3) + 1 */
        seq_t val_mod = mod_pow((seq_t)3, next_k, k3);
        seq_t val = val_mod + (seq_t)1;

        /* test primality */
        if (is_prime_seq(val)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    if (cache) free(cache);
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }

        ++next_k;
    }

    if (cache_len < n) return (seq_t)0;  /* value overflows seq_t or not enough data */
    return cache[n - 1];
}
