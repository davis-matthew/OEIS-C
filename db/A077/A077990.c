/*
 * A077990  Expansion of 1/(1 + 2*x + x^2 - x^3).
 * https://oeis.org/A077990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 27, i32 n <= 55, i64 n <= 115,
 *            i128 n <= 230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A077990
 *  Expansion of 1/(1 + 2*x + x^2 - x^3)
 *
 *  Recurrence derived from the generating function:
 *      (1 + 2x + x^2 – x^3) * Σ a(n) x^n = 1
 *
 *  ⇒ a(0) = 1
 *     a(1) = -2
 *     a(2) = 3
 *     a(n) = -2*a(n-1) - a(n-2) + a(n-3),  n ≥ 3
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;   /* number of allocated entries */
    static idx_t last = 0;       /* next index to be filled (0 .. last-1 are valid) */

    /* initialise cache on first call */
    if (cache == NULL)
    {
        /* store the first three terms */
        capacity = 3;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-2;
        cache[2] = (seq_t)3;
        last = 3;   /* indices 0,1,2 are now valid */
    }

    /* grow cache if needed */
    if (n >= capacity)
    {
        idx_t new_cap = capacity >= 3 ? capacity * 2 : 8;
        if (new_cap <= n + 1)
            new_cap = n + 1;   /* at least enough for n */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)        /* allocation failure – abort */
            exit(EXIT_FAILURE);
        cache = tmp;
        capacity = new_cap;
    }

    /* compute missing terms up to n */
    while (last <= n)
    {
        seq_t a_nm1 = cache[last - 1];
        seq_t a_nm2 = cache[last - 2];
        seq_t a_nm3 = cache[last - 3];

        /* recurrence: a(last) = -2*a(last-1) - a(last-2) + a(last-3) */
        seq_t val = -(seq_t)2 * a_nm1 - a_nm2 + a_nm3;
        cache[last] = val;
        ++last;
    }

    return cache[n];
}
