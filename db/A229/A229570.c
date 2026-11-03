/*
 * A229570  Primes of form p*q + 30, where p and q are consecutive primes.
 * https://oeis.org/A229570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 54 terms through n = 997 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes of the form p*q + 30, where p and q are consecutive primes (OEIS A229570). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"

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

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static uint64_t curr_p = 0;
static uint64_t curr_q = 0;
static bool initialized = false;

static void init_sequence(void) {
    if (initialized)
        return;
    curr_p = 2;
    curr_q = next_prime(curr_p);
    initialized = true;
}

static void ensure_cache_capacity(size_t need) {
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    if (cache == NULL) {
        cache = seq_alloc(new_cap);
    } else {
        cache = seq_realloc(cache, cache_cap, new_cap);
    }
    if (cache == NULL)
        abort(); /* memory allocation failed */
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0)            /* guard for 0‑based callers */
        return (seq_t)0;

    if (!initialized) {
        init_sequence();
        cache = seq_alloc(16);
        cache_cap = 16;
        cache_len = 0;
    }

    if ((idx_t)cache_len >= n)
        return cache[n - 1];

    while ((idx_t)cache_len < n) {
        seq_t cand = (seq_t)curr_p * (seq_t)curr_q + (seq_t)30;
        if (is_prime_big(cand)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = cand;
        }
        curr_p = curr_q;
        curr_q = next_prime(curr_q);
    }

    return cache[n - 1];
}
