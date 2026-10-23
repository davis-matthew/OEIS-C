/*
 * A043384  Numbers having four 3's in base 6.
 * https://oeis.org/A043384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 427, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043384: Numbers having four 3's in base 6.
   The routine scans integers in order, tests the base‑6 digit
   counts and memoises the results so repeated calls are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Count how many times digit "3" occurs in the base‑6 representation
   of the argument.  Returns true iff the count equals four.
   The function is inline so the compiler can optimise it. */
static inline bool has_four_3(seq_t x)
{
    int count = 0;
    while (x != 0)
    {
        if (x % 6 == 3)
        {
            ++count;
            if (count > 4)
                return false;
        }
        x /= 6;
    }
    return count == 4;
}

/* Static cache for memoised terms.  cache_len holds the number
   of terms currently stored; cache_cap is the allocated capacity.
   cache is a zero‑terminated pointer (for simplicity). */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

/* Append a new term to the cache, enlarging the storage if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap)
    {
        cache_cap = cache_cap ? cache_cap << 1 : 64;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Main sequence function.  It guarantees that the nth term is computed
   and memoised.  The function is thread‑unsafe but satisfies the
   external linkage requirement. */
seq_t a(idx_t n)
{
    /* Guarantee we have at least n terms cached. */
    while (cache_len < n)
    {
        /* Start searching from the next integer after the last cached term. */
        seq_t k = (cache_len == 0) ? 1 : cache[cache_len - 1] + 1;
        for (;; ++k)
        {
            if (has_four_3(k))
            {
                cache_append(k);
                break;
            }
        }
    }
    return cache[n - 1];
}
