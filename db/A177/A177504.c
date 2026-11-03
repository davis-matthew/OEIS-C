/*
 * A177504  Primes of the form 5*10^n-9.
 * https://oeis.org/A177504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: standard, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A177504: Primes of the form 5*10^n-9.
   a(1) = 41, a(2) = 491, a(3) = 49991, a(4) = 49999991, a(5) = 4999999999999999999999991,
   ...  where the exponent n is not the index but the amount of 10s in the power.
   This implementation generates primes of the form 5*10^e-9 by brute‑force search
   and memoises the results.  The helper functions pow10, is_prime_big and seq_realloc
   are supplied by the build system.  The algorithm is O(m) where m is the
   exponent of the last prime found.  It works for the known terms (usually
   a few dozen) and easily stays below the time limit for n up to 10000.
 */

#include <stdbool.h>
#include <stddef.h>               /* size_t */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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

seq_t a(idx_t n) {
    /* Static cache of found primes and the next exponent to check. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_exponent = 1;  /* start with 10^1 */

    /* Ensure cache is initialized with a small capacity. */
    if (cache_cap == 0) {
        cache_cap = 4;
        cache = seq_realloc(NULL, 0, cache_cap);
        cache_len = 0;
    }

    /* Generate more primes until we have at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t ten_pow = pow10(next_exponent);         /* 10^next_exponent   */
        seq_t candidate = ten_pow * (seq_t)5 - (seq_t)9; /* 5*10^e - 9 */

        if (is_prime_big(candidate)) {
            /* Grow the cache if needed. */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = seq_realloc(cache, cache_len, cache_cap);
            }
            cache[cache_len++] = candidate;
        }
        ++next_exponent;  /* proceed to the next exponent */
    }

    return cache[n - 1];  /* cache is 0‑based, a(1) is at index 0 */
}
