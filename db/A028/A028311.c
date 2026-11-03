/*
 * A028311  Odd numbers k such that {1..k-1} can be partitioned into disjoint
 *          sets I, J with 2I == -J (mod k) such that I, J are unions of
 *          cyclotomic cosets mod k.
 * https://oeis.org/A028311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   odd numbers k with no prime divisor ≡ 3 (mod 8)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts with 5, 7, 13, 17, 23, 25, 29, 31, 35, 37, 41, 47, 49, 53, 61 … */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether an odd integer k has a prime factor ≡ 3 (mod 8). */
static bool has_bad_prime(uint64_t k)
{
    /* k is odd and ≥ 5 */
    uint64_t t = k;
    /* Handle factor 3 explicitly */
    if (t % 3 == 0) {
        /* 3 ≡ 3 (mod 8) – bad prime */
        return true;
    }
    /* Test odd divisors d = 5,7,9,… up to √t */
    for (uint64_t d = 5; d * d <= t; d += 2) {
        if (t % d == 0) {
            if (d % 8 == 3) return true;      /* bad prime found */
            while (t % d == 0) t /= d;        /* remove all copies */
        }
    }
    /* t may be a prime > √original */
    if (t > 1 && (t % 8 == 3)) return true;
    return false;
}

static seq_t *cache = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t last_checked = 3;   /* odd number last examined */

/* Append a new term to the cache, growing the array if necessary. */
static void append_term(uint64_t v)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return;   /* ignore allocation failure – not expected */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = (seq_t)v;
}

/* Ensure the cache contains at least n elements. */
static void ensure_n(idx_t n)
{
    while ((idx_t)cache_len < n) {
        /* Advance to the next odd integer */
        last_checked += 2;
        if (!has_bad_prime(last_checked)) {
            append_term(last_checked);
        }
    }
}

/* Main function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 0 */
    ensure_n(n);
    return cache[n - 1];
}
