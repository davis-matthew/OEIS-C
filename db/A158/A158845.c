/*
 * A158845  Numbers k such that prepending 1 to the k-th triangular number
 *          produces a prime.
 * https://oeis.org/A158845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k such that concatenating 1 and the k-th
 *            triangular number T(k) = k(k+1)/2 yields a prime
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 941 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n) {
    /* static cache of already found terms */
    static seq_t *cache = NULL;        /* array of found k's */
    static size_t cache_size = 0;      /* current number of terms stored */
    static size_t cache_cap  = 0;      /* capacity of the cache */
    static idx_t  next_k     = 1;      /* next candidate k to test */

    if (!cache) {                      /* initialize on first call */
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        cache_size = 0;
    }

    while (cache_size < (size_t)n) {   /* generate until we have n terms */
        seq_t k = (seq_t)next_k;
        seq_t T = k * (k + 1) / 2;               /* triangular number */
        int d = num_digits(T);                    /* decimal digit count of T */
        seq_t candidate = pow10((idx_t)d) + T;    /* concatenate 1 and T */

        if (isPrimeGeneric(candidate)) {
            if (cache_size == cache_cap) {       /* grow cache if needed */
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_size++] = k;             /* store found k */
        }
        next_k++;                                 /* try next k */
    }

    return cache[n - 1];                          /* 1‑based indexing */
}
