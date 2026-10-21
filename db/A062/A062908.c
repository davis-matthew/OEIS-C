/*
 * A062908  Non-palindromic number and its reversal are both even.
 * https://oeis.org/A062908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer that is even, not palindromic,
 *            and whose decimal digit reversal is also even
 * Range:     u8 n <= 38, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_palindrome, rev, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#include "../utils/rev.h"
#include "../utils/is_palindrome.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Return the n-th term of A062908.  n >= 1. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static idx_t  candidate   = 1;  /* next integer to test */

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n)
    {
        /* Find the next valid number. */
        for (;; ++candidate)
        {
            if ((candidate & 1) == 0)                 /* candidate is even   */
            {
                idx_t rev_val = rev(candidate);      /* reverse its digits  */
                if ((rev_val & 1) == 0)              /* reversal is even   */
                {
                    if (!is_palindrome((seq_t)candidate, (idx_t)10))
                    {
                        /* candidate satisfies all conditions */
                        break;
                    }
                }
            }
        }

        /* Ensure the cache has space. */
        if (cache_size >= cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap << 1 : 8;
            while (new_cap <= cache_size)
                new_cap <<= 1;
            seq_t *new_cache = seq_alloc_new_array(new_cap);
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            if (cache)
                seq_free_array(cache);
            cache     = new_cache;
            cache_cap = new_cap;
        }

        /* Store the found term. */
        cache[cache_size] = (seq_t)candidate;
        ++cache_size;

        /* Advance to the next candidate for future searches. */
        ++candidate;
    }

    return cache[(size_t)n - 1];
}
