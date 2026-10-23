/*
 * A179909  Primes with only one embedded prime.
 * https://oeis.org/A179909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 409, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   modmul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search primes and test the condition that a prime has
   exactly one distinct prime substring (excluding the full number). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/modmul64.h"

/* --------- 64‑bit Miller–Rabin primality test ------------ */

static inline uint64_t modpow64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = modmul64(r, a, mod);
        a = modmul64(a, a, mod);
        d >>= 1;
    }
    return r;
}

static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small) / sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = modmul64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (cont) continue;
        return false;
    }
    return true;
}

/* --------- Test whether a prime has exactly one distinct
            prime substring (excluding itself) --------- */

static bool test_good_uint64(uint64_t p)
{
    /* extract decimal digits (most significant first) */
    int d[32];
    int len = 0;
    uint64_t t = p;
    if (t == 0) { d[0] = 0; len = 1; }
    else {
        while (t > 0 && len < 32) { d[len++] = t % 10; t /= 10; }
        for (int i = 0; i < len / 2; ++i) {
            int tmp = d[i];
            d[i] = d[len - 1 - i];
            d[len - 1 - i] = tmp;
        }
    }

    bool found = false;
    uint64_t first = 0;

    for (int i = 0; i < len; ++i) {
        uint64_t sub = 0;
        for (int j = i; j < len; ++j) {
            sub = sub * 10 + d[j];
            /* skip the full number itself */
            if (i == 0 && j == len - 1) continue;
            if (isPrime64(sub)) {
                if (!found) {
                    found = true;
                    first = sub;
                } else if (sub != first) {
                    return false;      /* more than one distinct prime */
                }
            }
        }
    }
    return found;                       /* exactly one distinct prime? */
}

/* --------- Static state for caching the sequence --------- */

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t candidate = 2;   /* next number to test for primality */

/* find the next prime and advance candidate */
static uint64_t next_prime(void)
{
    if (candidate <= 2) {
        uint64_t res = 2;
        candidate = 3;
        return res;
    }
    if ((candidate & 1) == 0) ++candidate;
    for (;;) {
        if (isPrime64(candidate)) {
            uint64_t res = candidate;
            candidate += 2;          /* next odd */
            return res;
        }
        candidate += 2;
    }
}

/* ----------------- Sequence function -------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; values for n<1 undefined */
    while (cache_len < n) {
        uint64_t p = next_prime();
        if (test_good_uint64(p)) {
            if (cache_cap == 0) {
                cache_cap = 64;
                cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            }
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)p;
        }
    }
    return cache[n - 1];
}
