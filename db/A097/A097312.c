/*
 * A097312  Numbers with property that can bring the first digit to the back
 *          of the number to get a prime (zeros are dropped).
 * https://oeis.org/A097312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the nth positive integer x such that rotating the first
 *            decimal * digit to the back (dropping leading zeros) yields a
 *            prime number. * Example: 1234 → 2341 is prime, so 1234 is in the
 *            sequence
 * Range:     u8 n <= 42, u16 n <= 3393, u32 n <= 11018, u64 n <= 11018,
 *            u128 n <= 11018, big any n
 * Validated: standard, pass, 76 terms through n = 8799 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
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

/* static cache for memoised terms of the sequence */
static seq_t *cache    = NULL;   /* array of stored terms */
static size_t cache_cap = 0;     /* allocated number of slots */
static size_t cache_cnt = 0;     /* number of terms stored */
static seq_t last_checked = 0;   /* largest integer examined so far */

/* Decide whether x satisfies the property. */
static bool good_number(seq_t x)
{
    /* number of decimal digits of x */
    size_t dlen = num_digits(x);

    /* 10^(dlen-1) */
    seq_t ten_pow = pow10((idx_t)(dlen - 1));

    /* most significant digit */
    seq_t first = x / ten_pow;

    /* remaining digits */
    seq_t rest  = x % ten_pow;

    /* rotate first digit to the back, dropping leading zeros */
    seq_t y = rest * (seq_t)10 + first;

    return isPrimeGeneric(y);
}

/* Return the nth term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset 1: undefined for n < 1 */

    /* Return cached value if already computed */
    if (cache && (size_t)n <= cache_cnt)
        return cache[(size_t)n - 1];

    /* Initialise cache lazily */
    if (!cache) {
        cache_cap = 8;                            /* arbitrary start capacity */
        cache = seq_alloc(cache_cap);
        cache_cnt = 0;
    }

    /* Search successive integers until we have enough terms */
    for (seq_t x = last_checked + 1; ; ++x) {
        if (good_number(x)) {
            /* Grow cache if necessary */
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap * 2 + 1;
                seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
                if (!new_cache) {                 /* OOM - fall back */
                    /* In a real system we would handle this */
                    return 0;
                }
                cache = new_cache;
                cache_cap = new_cap;
            }

            cache[cache_cnt++] = x;
            last_checked = x;

            if (cache_cnt >= (size_t)n)
                return cache[(size_t)n - 1];
        }
    }
}
