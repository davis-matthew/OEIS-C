/*
 * A037392  Every base 4 digit of n is a base 10 digit of n.
 * https://oeis.org/A037392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose every base‑4 digit is a
 *            base‑10 digit of that integer
 * Range:     u8 n <= 17, u16 n <= 2050, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"

/* Cached values of the sequence.  The cache is grown as needed. */
static seq_t *cache   = NULL;  /* array of found numbers */
static size_t cache_len = 0;   /* number of values currently stored */
static size_t cache_cap = 0;   /* current capacity of the array   */
static seq_t next_candidate = 1;   /* integer to test next */

/* Return true iff every base‑4 digit of x is a decimal digit of x. */
static bool valid(seq_t x)
{
    unsigned int mask = 0;  /* bit mask for decimal digits present */
    seq_t t = x;

    /* Construct mask of decimal digits of x */
    if (t == 0)
    {
        mask |= 1u << 0;   /* digit 0 */
    }
    else
    {
        while (t > 0)
        {
            unsigned d = (unsigned)(t % 10);
            mask |= 1u << d;
            t /= 10;
        }
    }

    /* Check each base‑4 digit against the mask */
    seq_t y = x;
    if (y == 0)
    {
        return (mask & 1u) != 0;   /* digit 0 must be present */
    }
    while (y > 0)
    {
        unsigned d = (unsigned)(y % 4);
        if ((mask & (1u << d)) == 0)
            return false;           /* required digit missing */
        y /= 4;
    }
    return true;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n = 0; return a dummy value */

    /* Grow the cache until we have at least n terms. */
    while (cache_len < (size_t)n)
    {
        if (valid(next_candidate))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp)
                    abort();                   /* allocation failed */
                cache   = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];   /* indices start at 1 */
}
