/*
 * A044415  Numbers n such that string 8,3 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044415: numbers n such that the decimal representation of n contains
 * the substring "83" but that of n-1 does not. The implementation builds terms
 * on demand using a search/cache approach. */
#include <stdlib.h>
#include <stdbool.h>

/* Return true iff x contains the consecutive digits 8 followed by 3. */
static bool contains_83(seq_t x)
{
    seq_t remaining = x;
    while (remaining >= (seq_t)10)
    {
        seq_t d   = remaining % (seq_t)10;
        seq_t nxt = (remaining / (seq_t)10) % (seq_t)10;
        if (d == (seq_t)3 && nxt == (seq_t)8)
            return true;
        remaining = remaining / (seq_t)10;
    }
    return false;
}

/* Static cache for already computed terms. */
static seq_t *cache    = NULL;  /* pointer to array of terms */
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_len = 0;   /* number of stored terms */
static seq_t current    = 0;   /* last value examined */

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0, return 0 as a fallback */

    /* Lazily initialise cache. */
    if (cache == NULL)
    {
        cache_cap = 1;
        cache    = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;   /* allocation failure (unlikely for test cases) */
        cache_len = 0;
        current   = 0;
    }

    /* Generate terms until we have at least n terms. */
    while (cache_len < n)
    {
        seq_t candidate = current + (seq_t)1;  /* first value to test */

        /* Search for the next acceptable number. */
        while (true)
        {
            if (contains_83(candidate) && !contains_83(candidate - (seq_t)1))
            {
                /* Store the found term into the cache. */
                if (cache_len == cache_cap)
                {
                    cache_cap <<= 1;  /* double capacity */
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  cache_cap * sizeof(seq_t));
                    if (!tmp)
                        return (seq_t)0;   /* allocation failure */
                    cache = tmp;
                }
                cache[cache_len++] = candidate;
                current = candidate;
                break;  /* exit inner search loop */
            }
            candidate = candidate + (seq_t)1;
        }
    }

    return cache[n - 1];  /* 0‑based array index */
}
