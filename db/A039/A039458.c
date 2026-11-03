/*
 * A039458  Numbers whose base-11 representation has the same nonzero number
 *          of 2's and 4's.
 * https://oeis.org/A039458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑11 representation has the same nonzero
   number of 2's and 4's. The implementation searches for terms
   of the sequence and memoises them so that any call to a(n) is
   O(1) after the sequence has been generated up to n. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a number has equal non‑zero counts of 2 and 4 in its
   base‑11 representation. */
static bool has_equal_2_4(seq_t value)
{
    idx_t count2 = 0, count4 = 0;
    seq_t v = value;
    while (v)           /* v > 0 */
    {
        seq_t digit = v % 11;
        if (digit == 2) count2++;
        else if (digit == 4) count4++;
        v = v / 11;
    }
    return (count2 > 0 && count2 == count4);
}

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache(idx_t needed)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < (size_t)needed) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* Append a new term to the cache. */
static void cache_append(seq_t val)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    if (cache_len >= cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;

    /* make cache_len visible to other functions */
    /* (cached data is kept in static storage) */
}

/* Retrieve the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, but safe */

    /* static storage for already found terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialise cache on first call */
    if (!cache)
    {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        cache_len = 0;
    }

    /* generate terms until we have the nth one */
    if (n > cache_len)
    {
        seq_t start = (cache_len == 0 ? 1 : cache[cache_len - 1] + 1);
        seq_t x = start;
        idx_t found = cache_len;

        while (found < n)
        {
            if (has_equal_2_4(x))
            {
                /* store the found term */
                if (found >= cache_cap)
                {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[found++] = x;
            }
            ++x;
        }
        cache_len = n;   /* we have at least n terms now */
    }

    return cache[n - 1];
}
