/*
 * A097269  Numbers that are the sum of two nonzero squares but not the
 *          difference of two nonzero squares.
 * https://oeis.org/A097269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097269 – Numbers that are the sum of two non‑zero squares
   but not the difference of two non‑zero squares.  The function
   generates terms by traversing odd integers m, checking whether m
   has any prime factor congruent to 3 (mod 4) to an odd power; if so
   m fails.  For each acceptable m the term 2·m is stored.  The cache
   is expanded lazily, so any call to a(n) returns the correct value
   irrespective of call order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether an odd integer m is expressible as the sum of two
   non‑zero squares.  This is equivalent to saying that every prime
   factor p ≡ 3 (mod 4) appears to an even exponent. */
static bool odd_good(idx_t m)
{
    if (m == 1)
        return true;

    idx_t temp = m;
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if ((p & 3) == 3 && (exp & 1) != 0)
                return false;
        }
    }
    /* Remaining prime factor larger than sqrt(m) */
    if (temp > 1 && (temp & 3) == 3)
        return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* behaviour below offset is irrelevant */

    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  next_candidate = 3;   /* next odd number to test */

    if (!cache) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        idx_t m;
        if (cache_len == 0) {
            m = 1;                          /* first odd number */
        } else {
            /* Find the next good odd m. */
            do {
                if (odd_good(next_candidate)) {
                    m = next_candidate;
                    next_candidate += 2;    /* advance past chosen m */
                    break;
                }
                next_candidate += 2;
            } while (1);
        }

        seq_t val = (seq_t)2 * (seq_t)m;     /* 2·m is the required term */

        if (cache_len >= cache_cap) {
            cache_cap <<= 1;                /* double size */
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        cache[cache_len++] = val;
    }

    return cache[n - 1];
}
