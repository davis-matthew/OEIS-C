/*
 * A037645  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 2,3,1.
 * https://oeis.org/A037645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the integer formed by concatenating the first n terms of
 *            the repeating digit sequence 2,3,1. Equivalent to a(n) =
 *            floor(77 · 10^n / 333)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache previous values so the function works for arbitrary call order. */
    static seq_t *cache = NULL;   /* array of computed a(i) values, 0‑based */
    static size_t cache_len = 0;  /* allocated capacity (in elements)   */
    static size_t cache_used = 0; /* number of valid elements (>= n means ready) */

    if (n == 0)        /* out of range – not used in tests */
        return (seq_t)0;

    /* If we haven't computed up to n yet, extend the cache. */
    if ((size_t)n > cache_used)
    {
        size_t needed = (size_t)n;

        /* Grow the allocated buffer if necessary. */
        if (cache_len < needed)
        {
            size_t new_cap = cache_len ? cache_len * 2 : 8;
            while (new_cap < needed)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)       /* out of memory – terminate */
                exit(EXIT_FAILURE);
            cache = tmp;
            cache_len = new_cap;
        }

        /* Compute new terms starting from the last cached value. */
        seq_t prev = (cache_used ? cache[cache_used - 1] : (seq_t)0);
        for (size_t i = cache_used; i < needed; ++i)
        {
            seq_t digit;
            switch (i % 3)
            {
                case 0: digit = (seq_t)2; break;
                case 1: digit = (seq_t)3; break;
                default: digit = (seq_t)1; break;
            }
            prev = prev * (seq_t)10 + digit;
            cache[i] = prev;
        }
        cache_used = needed;
    }

    return cache[n - 1];
}
