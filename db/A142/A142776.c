/*
 * A142776  Primes congruent to 49 mod 59.
 * https://oeis.org/A142776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 108, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 909 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n‑th prime congruent to 49 modulo 59.
 * The function lazily generates terms and caches them for
 * subsequent calls, so the order of calls does not matter.
 */

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

/* arithmetic constants as seq_t values */
#define STEP  ((seq_t)59)

/* initial cache capacity */
#define INITIAL_CAP 32

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ensure the cache has room for at least one more element */
static void ensure_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : INITIAL_CAP;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0)  /* out of memory */
            exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; undefined for 0 */
        return (seq_t)0;

    /* initialise cache on first use */
    if (!cache) {
        cache = mem_new(seq_t, INITIAL_CAP);
        if (!cache)
            exit(EXIT_FAILURE);
        cache_cap = INITIAL_CAP;
    }

    /* start from the first candidate or the next after the last stored prime */
    seq_t cand;
    if (cache_len == 0) {
        cand = (seq_t)59 * 2 + (seq_t)49;          /* 167 is the first term */
    } else {
        cand = cache[cache_len - 1] + STEP;
    }

    /* generate primes until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (is_prime_big(cand)) {
            ensure_capacity();
            cache[cache_len++] = cand;
        }
        cand += STEP;
    }

    return cache[n - 1];
}
