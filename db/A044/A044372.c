/*
 * A044372  Numbers n such that string 4,0 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044372 – numbers n whose decimal representation contains the
   substring "40" while (n-1) does not.  This implementation searches
   consecutively and caches previously found terms. */

#include <stdlib.h>

/* Test whether a decimal number contains the substring "40" */
static int contains_40(seq_t x)
{
    const seq_t ten  = 10;  /* any value > 9 works as a sentinel */
    const seq_t zero = 0;
    seq_t prev = ten;

    while (x > zero)
    {
        seq_t digit = x % 10;
        if (prev == zero && digit == (seq_t)4)
            return 1;
        prev = digit;
        x /= 10;
    }
    return 0;
}

seq_t a(idx_t n)
{
    /* Static cache of already discovered terms */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t  next_checked = 0;   /* next candidate to test */

    if (n == 0)            /* undefined for n < 1, safe fallback */
        return 0;

    /* Generate terms until the cache contains at least n items */
    while (cache_len < (size_t)n)
    {
        next_checked++;   /* examine next integer */

        if (contains_40((seq_t)next_checked) &&
            !contains_40((seq_t)(next_checked - 1)))
        {
            /* Grow the cache if necessary */
            if (cache_len >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }

            /* Store the new term */
            cache[cache_len++] = (seq_t)next_checked;
        }
    }

    return cache[n - 1];
}
