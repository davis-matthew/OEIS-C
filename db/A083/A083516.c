/*
 * A083516  Smallest palindromic multiple of 11, sum of whose digits at some
 *          stage is equal to n.
 * https://oeis.org/A083516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the smallest palindromic multiple of 11 such that
   the repeated digit‑sum (with base 10) leads to the specified index n.
   It progressively scans multiples of 11, assigns values to all n
   encountered along the way, and stops once the requested n has been
   found.

   The sequence satisfies a(1)=55, a(2)=11, a(3)=66, … as given in OEIS
   entry A083516. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_palindrome.h"   /* is_palindrome(seq_t x, idx_t base) */

static seq_t *cache = NULL;          /* cache[n] = a(n) for n >= 1 */
static size_t cap = 0;               /* current capacity of cache */
static size_t assigned = 0;          /* number of filled positions >0 */
static idx_t next_k = 1;             /* next integer used to build a multiple of 11 */

/* sum of decimal digits of x */
static seq_t digit_sum(seq_t x)
{
    seq_t s = 0;
    while (x > 0)
    {
        s += x % 10;   /* 10 is an int constant, safe to mix with seq_t */
        x /= 10;
    }
    return s;
}

/* enlarge the cache to at least needed items (indices 0..needed-1) */
static void ensure_cap(size_t needed)
{
    if (cap >= needed)
        return;
    size_t new_cap = cap ? cap * 2 : 128;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* out of memory */
    /* zero the new part */
    for (size_t i = cap; i < new_cap; ++i)
        new_cache[i] = (seq_t)0;
    /* shift if realloc moved the block */
    if (new_cache != cache) {
        free(cache);
    }
    cache = new_cache;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* offset 1 */

    /* make sure we have space for the requested index */
    ensure_cap((size_t)n + 1);

    /* keep scanning until we have found a value for this index */
    while (cache[(size_t)n] == 0)
    {
        seq_t m = (seq_t)11 * (seq_t)next_k;   /* current multiple of 11 */
        ++next_k;

        if (!is_palindrome(m, 10))        /* skip non‑palindromic ones */
            continue;

        seq_t val = digit_sum(m);
        while (1)
        {
            /* ensure we can index val */
            if ((size_t)val + 1 > cap)
                ensure_cap((size_t)val + 1);

            if (cache[(size_t)val] == 0)
            {
                cache[(size_t)val] = m;
                ++assigned;
            }

            if (val < 10)
                break;                    /* no further sum needed */
            val = digit_sum(val);
        }
    }

    return cache[(size_t)n];
}
