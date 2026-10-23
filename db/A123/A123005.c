/*
 * A123005  Expansion of g.f. x^2/(1-2*x-49*x^2).
 * https://oeis.org/A123005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 49*a(n-2), a(1)=0, a(2)=1 (OEIS A123005)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* 1‑based array of computed terms   */
static idx_t  max_n  = 0;       /* highest n for which cache[n] is valid */

/* Compute the nth term of the sequence.  n >= 1.  */
seq_t a(idx_t n)
{
    /* Handle the lower offset explicitly. */
    if (n == 0) return 0; /* unused but safe */

    /* Extend cache if necessary. */
    if (max_n < n)
    {
        idx_t new_cap = n + 1;             /* indices 0..n, index 0 unused */
        seq_t *new_cache;

        if (cache == NULL)
        {
            /* First allocation: zero all elements. */
            new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        }
        else
        {
            /* Grow the existing array.  realloc does not guarantee zero
               for newly allocated memory, so we clear it ourselves. */
            new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            for (idx_t i = max_n + 1; i < new_cap; ++i)
                new_cache[i] = 0;
        }

        if (new_cache == NULL)
        {
            /* Allocation failure – return 0 as a safe fallback. */
            return 0;
        }
        cache = new_cache;

        /* Initialise base values when they are first needed. */
        if (max_n < 1 && new_cap > 1)
            cache[1] = 0;
        if (max_n < 2 && new_cap > 2)
            cache[2] = 1;

        /* Compute any new terms beyond the previously computed range. */
        idx_t start = (max_n < 3) ? 3 : max_n + 1;
        const seq_t two      = (seq_t)2;
        const seq_t forty_nine = (seq_t)49;
        for (idx_t i = start; i <= n; ++i)
            cache[i] = two * cache[i - 1] + forty_nine * cache[i - 2];

        max_n = n;
    }

    return cache[n];
}
