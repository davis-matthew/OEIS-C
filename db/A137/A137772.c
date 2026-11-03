/*
 * A137772  Prime numbers p such that p +- ((p-1)/30030) are primes.
 * https://oeis.org/A137772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137772: prime p such that p ± ((p-1)/30030) are primes. */
/* The sequence lists all primes p with that property (offset = 1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

/* Cache for already found terms of the sequence. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

/* Current k (the multiplier) used to generate candidates p = 30030*k + 1. */
static idx_t next_k = 1;

/* Grow the cache when needed. */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    } else {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (tmp) {
            cache = tmp;
            cache_cap = new_cap;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Generate terms until the cache has at least n elements. */
    while (cache_size < (size_t)n) {
        seq_t k = (seq_t)next_k;
        seq_t p  = k * (seq_t)30030 + (seq_t)1;   /* 30030*k + 1 */
        seq_t q1 = k * (seq_t)30029 + (seq_t)1;   /* 30029*k + 1 = p - k */
        seq_t q2 = k * (seq_t)30031 + (seq_t)1;   /* 30031*k + 1 = p + k */

        if (is_prime_big(p) && is_prime_big(q1) && is_prime_big(q2)) {
            if (cache_size == cache_cap) grow_cache();
            cache[cache_size++] = p;
        }
        ++next_k;
    }
    return cache[n - 1];
}
