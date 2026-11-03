/*
 * A228359  Numbers m whose 10's complement is not prime, i.e., 10^k-m, where
 *          k is the number of digits of m, is not prime.
 * https://oeis.org/A228359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, less
 * Formula:   a(n) is the nth positive integer m such that 10^digits(m) – m is
 *            not prime
 * Range:     u8 n <= 120, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 84 terms through n = 992 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits0.h"
#include "../utils/mem.h"

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

static seq_t *cache        = NULL;  /* stored sequence values */
static size_t cache_cap    = 0;     /* allocated capacity of cache */
static idx_t  cache_cnt    = 0;     /* number of values stored in cache */
static idx_t  next_m       = 1;     /* next candidate integer to test */

/* Ensure cache has at least n entries. */
static void ensure_cache(idx_t n)
{
    while (cache_cnt < n) {
        seq_t cand = (seq_t)next_m;
        /* number of decimal digits of cand (stored in a 64‑bit type to avoid overflow) */
        idx_t d = (idx_t)num_digits0((uint64_t)next_m);
        seq_t pow = pow10(d);              /* 10^d as seq_t */
        seq_t comp = pow - cand;           /* 10^d – m */
        bool prime = isPrimeGeneric(comp);
        if (!prime) {                      /* composite or 1 */
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap? cache_cap * 2 : 1;
                grow_seq(&cache, &cache_cap, new_cap);
            }
            cache[cache_cnt++] = cand;
        }
        ++next_m;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1; return 0 as safe default */
    ensure_cache(n);
    return cache[n-1];
}
