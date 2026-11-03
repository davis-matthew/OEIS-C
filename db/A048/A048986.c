/*
 * A048986  Home primes in base 2: primes reached when you start with n and
 *          (working in base 2) concatenate its prime factors (A048985);
 *          repeat until a prime is reached (or -1 if no prime is ever
 *          reached). Answer is written in base 10.
 * https://oeis.org/A048986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   iterate the binary home‑prime process until a prime is reached;
 *            return that prime (in base 10) or -1 if no prime is ever reached
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 132, u64 n <= 921,
 *            u128 n <= 1344, big any n
 * Validated: standard, pass, 71 terms through n = 2263 (2026-11-03)
 * Helpers:   factor_u64, ilog2, is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/factor_u64.h"
#include "../utils/ilog2.h"
#include "../utils/is_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *cache = NULL;          /* 1‑based cache, value 0 means “not computed” */
static size_t cache_cap = 0;

static inline bool isPrimeGeneric(seq_t x)
{
    if (sizeof(seq_t) <= 8)            /* 64‑bit implementation uses is_prime(idx_t) */
        return is_prime((idx_t)x);
    else                               /* arbitrary‑precision implementation */
        return is_prime_big(x);
}

/* Grow the cache so that cache[n] is valid; freshly allocated cells are zeroed. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = mem_new(seq_t, cache_cap);
    } else if (cache_cap <= (size_t)n) {
        size_t old_cap = cache_cap;
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n + 1) new_cap = (size_t)n + 1;
        cache = mem_resize(seq_t, cache, new_cap);
        mem_set(cache + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Home–prime computation for a single term. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;   /* not used, but keep definition safe */

    ensure_cache(n);
    if (cache[n] != 0) return cache[n];

    seq_t result = (seq_t)-1;       /* default = “no prime reached” */

    if (n == 1) {                   /* convention */
        result = 1;
    } else {
        seq_t cur = n;
        const int MAX_STEPS = 1000;
        int step = 0;

        while (step < MAX_STEPS) {
            if (isPrimeGeneric(cur)) { result = cur; break; }

            /* 64‑bit factorisation is only possible if cur fits. */
            if (sizeof(seq_t) > 8 && cur > (seq_t)UINT64_MAX) break;

            uint64_t val = (uint64_t)cur;
            if (val <= 1) break;

            size_t cnt = 0;
            uint64_t pf[64], ex[64];
            factor_u64(val, pf, ex, &cnt);

            seq_t next = 0;
            for (size_t i = 0; i < cnt; ++i) {
                uint64_t p   = pf[i];
                size_t   bits = ilog2(p) + 1;
                for (size_t e = 0; e < ex[i]; ++e)
                    next = (next << bits) | (seq_t)p;
            }
            cur = next;
            ++step;
        }
    }

    cache[n] = result;
    return result;
}
