/*
 * A044420  Numbers n such that string 8,8 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044420: Numbers n such that string 8,8 occurs in the base‑10 representation of n but not of n‑1.
   This implementation searches sequentially, storing found terms in a static cache so
   repeated calls (in any order) are efficient. */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* Helper: does the decimal representation of x contain two adjacent 8 digits? */
static bool has_adjacent_8s(seq_t x)
{
    seq_t prev = 10;                 /* sentinel larger than any digit */
    while (x != 0)
    {
        seq_t digit = x % 10;
        if (digit == 8 && prev == 8)
            return true;
        prev = digit;
        x /= 10;
    }
    return false;
}

/* Dynamically grow the cache array when needed. */
static void grow_cache(size_t *cap, seq_t **cache, size_t len)
{
    size_t new_cap = (*cap) ? (*cap) * 2 : 16;
    seq_t *new_cache = mem_new(seq_t, new_cap);
    if (*cache)
    {
        mem_copy(new_cache, *cache, len * sizeof(seq_t));
        mem_free(*cache);
    }
    *cache = new_cache;
    *cap = new_cap;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    /* Static cache to hold the sequence terms found so far. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_checked = 0;
    static bool initialized = false;

    if (!initialized)
    {
        /* Lazy initialization. */
        cache_len = 0;
        cache_cap = 0;
        cache = NULL;
        last_checked = 0;
        initialized = true;
    }

    if (n == 0)          /* offset is 1; for safety, return 0 for n==0. */
        return 0;

    /* Extend cache until we have at least n terms. */
    while (cache_len < (size_t)n)
    {
        seq_t curr = last_checked + 1;
        while (true)
        {
            if (has_adjacent_8s(curr) && !has_adjacent_8s(curr - 1))
            {
                /* Store the term. */
                if (cache_len >= cache_cap)
                    grow_cache(&cache_cap, &cache, cache_len);
                cache[cache_len++] = curr;
                last_checked = curr;
                break;
            }
            ++curr;
        }
    }

    return cache[n - 1];
}
