/*
 * A039152  Numbers whose base-10 representation has the same number of 6's
 *          and 9's.
 * https://oeis.org/A039152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A039152 – Numbers whose base‑10 representation has the same
   number of 6's and 9's.  
   The function generates the terms on demand, caching them in a static
   dynamic array.  The nth term (OEIS indexing, n ≥ 1) is returned.
 */
#include <stdlib.h>
#include <stdint.h>

static seq_t* cache = NULL;      /* array of computed terms          */
static size_t cache_size = 0;    /* number of terms stored           */
static size_t cache_cap  = 0;    /* allocated capacity               */
static idx_t  next_test = 0;     /* next integer to examine          */

/* Grow the cache so that it contains at least 'cap' elements.          */
static void grow_cache(size_t cap)
{
    if (cache_cap >= cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < cap)
        new_cap <<= 1;
    /* cast realloc's void* to seq_t* (valid in both C & C++)            */
    seq_t* tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Build the cache until it contains n terms.                       */
    while (cache_size < (size_t)n)
    {
        /* Count 6's and 9's in decimal representation of next_test.   */
        unsigned count6 = 0, count9 = 0;
        if (next_test != 0)
        {
            idx_t x = next_test;
            while (x > 0)
            {
                unsigned d = x % 10;
                if (d == 6)
                    ++count6;
                else if (d == 9)
                    ++count9;
                x /= 10;
            }
        }
        /* 0 has 0 sixes and 0 nines – equal.                            */
        if (count6 == count9)
        {
            grow_cache(cache_size + 1);
            cache[cache_size++] = (seq_t)next_test;
        }
        ++next_test;
    }
    /* Return the (n‑1)th element of the cache (OEIS uses 1‑based indexing). */
    return cache[(size_t)n - 1];
}
