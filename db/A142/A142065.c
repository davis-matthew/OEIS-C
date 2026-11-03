/*
 * A142065  Primes congruent to 28 mod 33.
 * https://oeis.org/A142065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 326, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 963 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes p ≡ 28 (mod 33) by incremental search. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
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

/* --------------------------------------------------------------------- */
/*  Static cache for the sequence values.                                 */
/* --------------------------------------------------------------------- */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t  next_k     = 1;   /* next value of k in 33*k+28 */
static bool   initialized = false;

/* --------------------------------------------------------------------- */
static void init_cache(void)
{
    if (initialized) return;
    cache = seq_alloc(16);
    cache_cap = 16;
    cache_len = 0;
    next_k = 1;
    initialized = true;
}

/* --------------------------------------------------------------------- */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* zero for indices before the offset */

    init_cache();

    while (cache_len < (size_t)n) {
        /* candidate of the form 33*k + 28 */
        uint64_t cand_uint64 = 33 * (uint64_t)next_k + 28ULL;
        seq_t cand = (seq_t)cand_uint64;

        if (is_prime_big(cand)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = cand;
        }
        ++next_k;
    }

    return cache[n - 1];
}
