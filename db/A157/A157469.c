/*
 * A157469  Positive numbers y such that y^2 is of the form x^2 + (x+97)^2
 *          with integer x.
 * https://oeis.org/A157469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n > 6; a(1)=85, a(2)=97, a(3)=113,
 *            a(4)=397, a(5)=485, a(6)=593
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 32, u64 n <= 69, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n)
{
    /* 1‑based cache; cache[0] is unused */
    static seq_t *cache = NULL; /* pointer to cached terms */
    static idx_t last = 0;      /* highest index currently cached */

    if (n == 0)  /* undefined for offset 1; return 0 as a guard */
        return (seq_t)0;

    /* Grow the cache if needed */
    if (n > last)
    {
        size_t new_size = (size_t)n + 1;            /* include unused 0 index */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                          /* allocation failure */
        cache = tmp;

        /* Compute new entries */
        for (idx_t i = last + 1; i <= n; ++i)
        {
            if (i == 1)       cache[i] = (seq_t)85;
            else if (i == 2)  cache[i] = (seq_t)97;
            else if (i == 3)  cache[i] = (seq_t)113;
            else if (i == 4)  cache[i] = (seq_t)397;
            else if (i == 5)  cache[i] = (seq_t)485;
            else if (i == 6)  cache[i] = (seq_t)593;
            else
            {
                /* All arithmetic is performed in seq_t to keep
                   arbitrary‑precision operations correct. */
                cache[i] = cache[i - 3] * (seq_t)6 - cache[i - 6];
            }
        }
        last = n;
    }

    return cache[n];
}
