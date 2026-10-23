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
 * Helpers:   has_bad_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts with 5, 7, 13, 17, 23, 25, 29, 31, 35, 37, 41, 47, 49, 53, 61 … */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_bad_prime.h"

static seq_t *cache = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t last_checked = 3;   /* odd number last examined */

/* Append a new term to the cache, growing the array if necessary. */
static void append_term(uint64_t v)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
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
