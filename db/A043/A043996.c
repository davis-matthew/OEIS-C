/*
 * A043996  Numbers k such that 0 and 1 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A043996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043996: numbers k such that 0 and 1 occur juxtaposed in k but not in k+1.
   The function generates terms on demand, memoising results for arbitrary
   order of calls. */
 
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static bool has_adjacent_zero_one(seq_t x)
{
    /* Return true if the decimal representation of x contains the digits
       0 and 1 adjacent in either order. */
    if (x < (seq_t)10) return false;   /* Only one digit → impossible */

    seq_t prev = x % (seq_t)10;   /* least significant digit */
    x /= (seq_t)10;
    seq_t curr;

    while (x > (seq_t)0)
    {
        curr = x % (seq_t)10;
        if ( (curr == (seq_t)0 && prev == (seq_t)1) ||
             (curr == (seq_t)1 && prev == (seq_t)0) )
            return true;
        prev = curr;
        x /= (seq_t)10;
    }
    return false;
}

static seq_t *cache          = NULL;   /* array of found terms */
static size_t  cache_cap     = 0;       /* allocated size of cache          */
static size_t  cache_len     = 0;       /* number of terms cached           */
static seq_t   scan_next     = (seq_t)1;/* next integer to test             */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

    if (cache == NULL)
    {
        /* initialise cache */
        cache_cap = 16;
        cache = seq_alloc(cache_cap);
        cache_len = 0;
        scan_next = (seq_t)1;
    }

    while (cache_len < (size_t)n)
    {
        if (has_adjacent_zero_one(scan_next))
        {
            seq_t nxt = scan_next + (seq_t)1;
            if (!has_adjacent_zero_one(nxt))
            {
                if (cache_len >= cache_cap)
                {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = scan_next;
            }
        }
        scan_next += (seq_t)1;
    }
    return cache[(size_t)n - 1];
}
