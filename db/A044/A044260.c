/*
 * A044260  Numbers k such that string 1,0 occurs in the base 9 representation
 *          of k but not of k-1.
 * https://oeis.org/A044260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 712, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044260: numbers k such that the base‑9 representation
   of k contains the substring "10" while (k-1) does not.
   The function generates terms by scanning integers, caching
   results so that a(n) can be called in any order.  The search
   stops once at least n terms have been found. */

#include <stdlib.h>
#include <stdbool.h>

/* Helper that returns true iff the base‑9 representation of x
   contains the substring 1,0. */
static bool has10(seq_t x)
{
    const seq_t nine = (seq_t)9;
    const seq_t one  = (seq_t)1;
    const seq_t zero = (seq_t)0;

    seq_t prev = x % nine;
    x /= nine;
    while (x > zero)
    {
        seq_t cur = x % nine;
        if (cur == one && prev == zero)
            return true;
        prev = cur;
        x /= nine;
    }
    return false;
}

/* Cached sequence: cache[0] unused, cache[1] is a(1), etc. */
static seq_t *cache      = NULL;
static size_t cache_sz   = 0; /* number of stored terms plus one */
static size_t cache_cap  = 0;
static seq_t cur_k       = (seq_t)1; /* next candidate to test */

/* Return the n-th term of the sequence (n >= 1).  The function
   builds the cache on first call and extends it as necessary. */
seq_t a(idx_t n)
{
    /* lazy initialization */
    if (!cache)
    {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0; /* unused sentinel */
        cache_sz = 1;        /* we have one unused slot */
    }

    /* Build the cache until we have at least n terms */
    while (cache_sz <= n)
    {
        if (has10(cur_k))
        {
            seq_t km1 = cur_k - (seq_t)1;
            if (!has10(km1))
            {
                /* ensure capacity */
                if (cache_sz == cache_cap)
                {
                    cache_cap *= 2;
                    seq_t *temp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!temp) abort();
                    cache = temp;
                }
                cache[cache_sz++] = cur_k;
            }
        }
        ++cur_k;
    }

    return cache[n];
}
