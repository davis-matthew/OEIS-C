/*
 * A044645  Numbers k such that string 1,4 occurs in the base 9 representation
 *          of k but not of k+1.
 * https://oeis.org/A044645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 890, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 989 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044645: numbers k such that the base‑9 representation of k contains
   the consecutive digits "1" followed by "4", but (k+1) does not. */

/* The implementation scans integers, stores the found terms in a
   static cache, and serves out‑of‑order queries. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether seq_t number `x` has the substring "14" in its
   base‑9 representation. Digits are extracted least‑significant
   first; the pattern "1""4" corresponds to a digit of 1 followed
   immediately (more significant) by a digit of 4. */
static bool has_14(seq_t x)
{
    uint8_t digits[64];   /* 128‑bit numbers have <= 40 base‑9 digits */
    size_t sz = 0;
    while (x > 0) {
        digits[sz++] = (uint8_t)(x % 9);
        x /= 9;
    }
    for (size_t i = 1; i < sz; ++i)
        if (digits[i] == 1 && digits[i-1] == 4)
            return true;
    return false;
}

/* Cache of discovered terms */
static seq_t *cache    = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_k = 1;

/* Ensure the cache contains at least `need` terms (1‑based index). */
static void ensure_cache(idx_t need)
{
    if (cache_len >= (size_t)need)
        return;

    /* Grow the cache capacity if necessary. */
    if (cache_cap < (need + 1) * 2) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (need + 1) * 2)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();          /* allocation failure */
        cache    = tmp;
        cache_cap = new_cap;
    }

    /* Scan forward until enough terms are found. */
    while (cache_len < (size_t)need) {
        seq_t k = next_k++;
        if (has_14(k) && !has_14(k + 1))
            cache[cache_len++] = k;
    }
}

/* Public function: a(idx_t n) returns the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for non‑positive indices */
    ensure_cache(n);
    return cache[n - 1];
}
