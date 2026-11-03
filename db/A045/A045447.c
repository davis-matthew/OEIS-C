/*
 * A045447  Primes congruent to {0, 5} mod 7.
 * https://oeis.org/A045447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 936 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: nth prime congruent to 0 or 5 modulo 7.
   The first two terms are 5 and 7; thereafter we list primes ≡5 (mod 7). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

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

/* Static cache for memoisation */
static seq_t *cache = NULL;          /* array of cached terms                     */
static size_t cache_cnt = 0;         /* number of terms stored so far             */
static size_t cache_cap = 0;         /* current capacity of the array             */
static bool   initialized = false;   /* have we performed one‑time initialisation? */
static uint64_t next_candidate = 9;   /* odd candidate after the initial terms */

/* Ensure the cache array can hold at least 'need' elements.
   Uses the utility 'grow_seq' – it reallocates while preserving contents. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap < need)
        grow_seq(&cache, &cache_cap, need);
}

/* One‑time initialisation of the cache with the first two terms. */
static void init_cache(void)
{
    if (initialized) return;

    /* Allocate initial capacity and insert the first two known terms. */
    cache_cap = 8;
    cache = mem_new(seq_t, cache_cap);
    if (!cache) abort();                  /* out of memory */
    cache[0] = (seq_t)5;
    cache[1] = (seq_t)7;
    cache_cnt = 2;

    /* The next odd number to test for being prime. */
    next_candidate = 9;
    initialized = true;
}

seq_t a(idx_t n)
{
    init_cache();

    /* Extend the cache until it contains the nth term. */
    while (cache_cnt < n)
    {
        ensure_cache_capacity(cache_cnt + 1);

        /* Find the next prime congruent to 5 (mod 7). */
        for (;;)
        {
            uint64_t p = next_candidate;
            next_candidate = p + 2;              /* skip to the next odd candidate */

            if (is_prime_big((seq_t)p) && (p % 7ULL == 5ULL))
            {
                cache[cache_cnt++] = (seq_t)p;
                break;   /* term added – proceed to generate further terms if needed */
            }
            /* otherwise keep searching */
        }
    }

    return cache[n - 1];   /* idx_t is 1‑based, cache is 0‑based */
}
