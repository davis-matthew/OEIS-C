/*
 * A194954  Slowest increasing sequence of primes such that a(1)=2,
 *          a(n)-a(n-1) is multiple of A000120(n-1).
 * https://oeis.org/A194954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1996, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 8622 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A194954: slowest increasing sequence of primes.
 *  a(1)=2, and for n>1 a(n) is the smallest prime > a(n-1) such that
 *  a(n)-a(n-1) is a multiple of the binary weight (popcount) of n-1. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* popcount of an idx_t value */
static inline idx_t popcount_idx(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll((unsigned long long)x);
#else
    /* portable fallback */
    idx_t c = 0;
    while (x) {
        c += x & 1;
        x >>= 1;
    }
    return c;
#endif
}

/* memoized sequence storage */
static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* current capacity */

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1; behaviour below offset is irrelevant */
    if (n == 0) return 0;  /* safety for C */

    /* extend cache up to n */
    while (cache_len < n) {
        idx_t idx = cache_len + 1;          /* desired index to compute */
        idx_t weight = popcount_idx(idx - 1); /* binary weight of n-1 */

        /* previous term */
        seq_t prev = (cache_len == 0) ? 2 : cache[cache_len - 1];
        if (cache_len == 0) {               /* initialise first term */
            /* a(1) = 2, already defined as prev */
            /* ensure capacity for first term */
            if (cache_cap == 0) {
                cache_cap = 4;
                cache = mem_new(seq_t, cache_cap);
            }
            cache[0] = 2;
            cache_len = 1;
            continue;
        }

        /* find smallest prime > prev with difference divisible by weight */
        seq_t candidate = prev + 1;
        for (;;) {
            if (is_prime_big(candidate)) {
                seq_t diff = candidate - prev;
                if ((diff % (seq_t)weight) == 0)
                    break;
            }
            candidate = candidate + 1;
        }

        /* ensure capacity */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort(); /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = candidate;
    }

    return cache[n - 1];
}
