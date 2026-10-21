/*
 * A218735  Values of x in the solutions to x^2 - 3xy + y^2 + 29 = 0, where 0
 *          < x < y.
 * https://oeis.org/A218735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) - a(n-4) with initial values: a(1)=5, a(2)=6,
 *            a(3)=9, a(4)=13
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: store computed terms (1‑based index) */
    static seq_t *cache = NULL;
    static size_t last = 0;          /* highest index computed so far */
    static size_t cap   = 0;          /* allocated capacity */

    /* --- initialisation ------------------------------------------ */
    if (cache == NULL)
    {
        /* allocate space for indices 0..4 (index 0 unused) */
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (cache == NULL)
            return 0;                /* allocation failure, unlikely in tests */

        /* dummy zero at index 0 */
        cache[0] = 0;

        /* seed the first four terms */
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)13;

        last = 4;
        cap  = 5;                    /* capacity covers indices 0..4 */
    }

    /* --- extend capacity if needed -------------------------------- */
    if ((size_t)n > last && ((size_t)n + 1) > cap)
    {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;          /* double twice until enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return 0;                /* realloc failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* --- compute missing terms using the recurrence ---------------- */
    for (size_t i = last + 1; i <= (size_t)n; ++i)
    {
        if (i <= 4)
        {
            /* already seeded */
        }
        else
        {
            /* a(i) = 3*a(i-2) - a(i-4) */
            cache[i] = (seq_t)3 * cache[i - 2] - cache[i - 4];
        }
    }
    last = (size_t)n;
    return cache[n];
}
