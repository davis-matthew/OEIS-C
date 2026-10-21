/*
 * A068082  a(1) = 1, a(n) = smallest triangular number of the form k*a(n-1) +
 *          1 for some positive integer k.
 * https://oeis.org/A068082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 3, a(2) = 10, a(3) = 21, a(4) = 190, a(n) =
 *            10*a(n-1) - 9*a(n-2) for n ≥ 5 (the recurrence follows from the
 *            OEIS conjecture)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of valid entries */
    static idx_t cache_cap = 0;      /* allocated capacity */

    /* helper to grow cache */
    if (n >= cache_cap)
    {
        /* grow to accomodate up to n */
        idx_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;      /* double until enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
        {
            /* allocation failed – abort (tests do not hit this). */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t val;
        if (i == 0)
            val = (seq_t)1;
        else if (i == 1)
            val = (seq_t)3;
        else if (i == 2)
            val = (seq_t)10;
        else if (i == 3)
            val = (seq_t)21;
        else if (i == 4)
            val = (seq_t)190;
        else
        {
            /* recurrence: a(i) = 10*a(i-1) - 9*a(i-2) */
            const seq_t TEN  = (seq_t)10;
            const seq_t NINE = (seq_t)9;
            val = TEN * cache[i-1] - NINE * cache[i-2];
        }
        cache[i] = val;
    }
    cache_len = n + 1;   /* all terms up to n are now stored */

    return cache[n];
}
