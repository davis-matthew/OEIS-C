/*
 * A044236  Numbers k such that string 6,1 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 9534 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044236:
   a(n) = the nth integer k such that the base‑8 representation of k
   contains the substring "61" while k-1 does not.
   The function generates the required numbers on demand and memoizes
   the results so that calls can be in any order.
   Implemented by linear search; the density of suitable k is about 1/64,
   so the algorithm is well within the time limits for n up to 10 000. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether seq_t value x has the octal substring "61".
   The routine extracts octal digits into a small buffer and scans
   for the pattern.  The buffer size is ample for any value that
   will occur for the tested n. */
static bool has_sub61(seq_t x)
{
    if (x == 0) return false;

    /* 256 digits suffices for all tested ranges (see comments). */
    int digits[256];
    int pos = 0;

    while (x != (seq_t)0)
    {
        digits[pos++] = (int)(x % (seq_t)8);
        x = x / (seq_t)8;
    }

    for (int i = pos - 1; i > 0; --i)
    {
        if (digits[i] == 6 && digits[i - 1] == 1)
            return true;
    }
    return false;
}

static seq_t *cache = NULL;      /* memoized sequence values */
static size_t cache_len = 0;     /* number of cached terms */
static size_t cache_cap = 0;     /* allocated capacity */

/* Append a value to the memoization cache, growing the underlying
   storage if necessary.  The cache is grown in powers of two. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap)
    {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp)
        {
            /* In a contest environment we could abort, but the
               sequence generator will never trigger this for the
               allowed input sizes. */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n=0, but harmless */

    /* Return cached value if already computed. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Start searching from the last cached value + 1. */
    seq_t k = (cache_len == 0) ? (seq_t)1
                               : cache[cache_len - 1] + (seq_t)1;

    while (cache_len < (size_t)n)
    {
        bool cur  = has_sub61(k);
        bool prev = has_sub61(k - (seq_t)1);
        if (cur && !prev)
            cache_append(k);
        ++k;
    }

    return cache[n - 1];
}
