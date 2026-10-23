/*
 * A041013  Reverse or double: if reverse of a(n) > a(n), then a(n+1) = a(n)
 *          reversed, otherwise a(n+1) = 2*a(n).
 * https://oeis.org/A041013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, nice
 * Formula:   Reverse or double: a(0) = 1, a(n+1) = reverse(a(n)) if
 *            reverse(a(n)) > a(n) else 2 * a(n)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 56, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Helper: reverse decimal digits of a seq_t number */
static seq_t reverse_decimal(seq_t v)
{
    seq_t rev = 0;
    while (v != 0)
    {
        rev = rev * 10 + (v % 10);
        v   = v / 10;
    }
    return rev;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;
    static idx_t   max_n = 0;

    /* initialise cache with first term a(0) = 1 */
    if (cache == NULL)
    {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 1;
        max_n = 0;
    }

    /* extend cache up to n if needed */
    if (n > max_n)
    {
        size_t new_cap = cap;
        while (n >= new_cap)
            new_cap <<= 1;

        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;

        for (idx_t i = max_n + 1; i <= n; ++i)
        {
            seq_t prev = cache[i - 1];
            seq_t rev  = reverse_decimal(prev);
            if (rev > prev)
                cache[i] = rev;
            else
                cache[i] = prev * 2;
        }

        max_n = n;
    }

    return cache[n];
}
