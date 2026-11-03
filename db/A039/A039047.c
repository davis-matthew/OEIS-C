/*
 * A039047  Numbers whose base-7 representation has the same number of 4's and
 *          5's.
 * https://oeis.org/A039047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th non‑negative integer whose base‑7 representation
 *            contains the same number of digit 4’s as digit 5’s
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 8856 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if the base‑7 representation of x has equal 4’s and 5’s. */
static bool has_equal_4_5(seq_t x)
{
    unsigned int cnt4 = 0, cnt5 = 0;
    seq_t tmp = x;
    /* 0 has zero 4’s and zero 5’s, therefore equal. */
    if (tmp == 0u)
        return true;
    while (tmp != 0u) {
        seq_t digit = tmp % 7u;
        if (digit == 4u)
            ++cnt4;
        else if (digit == 5u)
            ++cnt5;
        tmp /= 7u;
    }
    return cnt4 == cnt5;
}

/* Static state to cache computed values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t candidate = 0;

/* Ensure the cache can hold at least 'need' elements.  Capacity is
   doubled each time for amortized linear growth. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_ptr = mem_resize(seq_t, cache, new_cap);
    cache = new_ptr;
    cache_cap = new_cap;
}

/* a(n) as defined in the OEIS.  n >= 1, but n == 0 returns 0. */
seq_t a(idx_t n)
{
    if (n == 0u)
        return 0u;

    /* Generate values until we have at least n terms cached. */
    while (cache_len < n) {
        if (cache_len >= cache_cap)
            ensure_cache_cap(cache_len + 1);

        if (has_equal_4_5(candidate))
            cache[cache_len++] = candidate;

        ++candidate;
    }

    return cache[n - 1];
}
