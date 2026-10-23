/*
 * A039416  Numbers whose base-10 representation has the same nonzero number
 *          of 2's and 9's.
 * https://oeis.org/A039416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth number whose decimal representation has a non‑zero
   equal number of 2's and 9's.  The sequence is defined by searching
   the positive integers and picking those that satisfy the condition.
   The function caches results between calls so that any order of
   requests for n is answered in essentially linear time. */

#include <stdint.h>
#include <stdbool.h>

/* Make sure we have access to the utility allocator for seq_t arrays */
#include "../utils/seq_alloc_new_array.h"

/* Forward declaration of the cache reallocation helper */
static void ensure_cache_capacity(size_t needed);

/* Static cache to store already generated terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently stored */
static size_t cache_cap = 0;   /* current capacity of the cache */
static uint64_t next_candidate = 1;   /* next integer to test */

/* Helper: grow the cache to at least 'needed' capacity.
   The cache is a plain array of seq_t objects. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? (cache_cap * 2) : 8;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_arr = seq_alloc_new_array(new_cap);
    /* copy existing entries */
    for (size_t i = 0; i < cache_len; ++i)
        new_arr[i] = cache[i];

    /* release old array */
    if (cache != NULL)
        free(cache);  /* sequence elements have trivial destructors in C,
                        * for C++ arbitrary‐precision types the allocator
                        * ensures proper destruction. */
    cache = new_arr;
    cache_cap = new_cap;
}

/* Main function required by the build.  Returns the nth term (1‑based). */
seq_t a(idx_t n)
{
    /* sanity check: sequence starts at index 1 */
    if (n == 0)
        return (seq_t)0;

    /* check if already computed */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* generate terms until we have at least n terms */
    while (cache_len < (size_t)n)
    {
        /* search for next candidate */
        uint64_t val = next_candidate++;
        if (val == 0)  /* wraparound check, very unlikely for current n */
            continue;

        /* count digits 2 and 9 */
        uint64_t tmp = val;
        unsigned cnt2 = 0, cnt9 = 0;
        while (tmp > 0)
        {
            uint8_t digit = tmp % 10;
            if (digit == 2)
                ++cnt2;
            else if (digit == 9)
                ++cnt9;
            tmp /= 10;
        }

        if (cnt2 > 0 && cnt2 == cnt9)
        {
            /* resize cache if necessary */
            ensure_cache_capacity(cache_len + 1);
            /* store the new term */
            cache[cache_len++] = (seq_t)val;
        }
    }

    return cache[(size_t)n - 1];
}
