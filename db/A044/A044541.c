/*
 * A044541  Numbers n such that string 3,1 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A044541 – numbers n for which the base‑7 representation of n
 * contains the substring “31” but the base‑7 representation of n+1 does not.
 *
 * The function searches sequentially from the last examined number,
 * caching results so that calls in any order remain correct.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool has31_base7(seq_t x)
{
    seq_t prev = 7;                 /* sentinel (cannot be a base‑7 digit) */
    while (x > 0) {
        seq_t cur = x % 7;          /* least significant digit */
        if (cur == 3 && prev == 1)  /* pattern 3 (more significant), 1 (less) */
            return true;
        prev = cur;
        x = x / 7;                  /* move to next more significant digit */
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Handle the offset: the sequence is defined for n ≥ 1. */
    if (n == 0)
        return 0;                   /* undefined value for n < 1 */

    /* Static cache for already discovered terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;     /* number of elements stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static seq_t last_checked = 0;   /* largest integer that has been examined */

    /* Return cached value if already computed. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Allocate or grow the cache if needed. */
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
    } else if (cache_len >= cache_cap) {
        cache_cap <<= 1;
        cache = mem_resize(seq_t, cache, cache_cap);
    }

    /* Search further numbers until we have at least n terms. */
    seq_t k = last_checked + 1;     /* start from the next integer after last check */
    while (cache_len < n) {
        if (has31_base7(k) && !has31_base7(k + 1)) {
            if (cache_len == cache_cap) {    /* grow if necessary */
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = k;
        }
        ++k;
    }

    last_checked = k - 1;            /* last examined integer */

    return cache[n - 1];
}
