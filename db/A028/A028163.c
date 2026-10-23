/*
 * A028163  Expansion of 1/((1-4x)(1-9x)(1-11x)(1-12x)).
 * https://oeis.org/A028163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028163
 *  a(0) = 1
 *  a(1) = 36
 *  a(n) = 23*a(n-1) - 132*a(n-2) + (9^(n+1) - 4^(n+1))/5   for n >= 2
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static idx_t max_n = (idx_t)-1;          /* highest n computed, -1 means not yet initialised */
    static seq_t *cache   = NULL;             /* pointer to array of computed a(i) */
    static seq_t pow4i = 1;                   /* 4^current_i   (i is the loop index in the filling loop) */
    static seq_t pow9i = 1;                   /* 9^current_i   */

    /* initialise on first call */
    if (max_n == (idx_t)-1)
    {
        /* we know a(0) and a(1) */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)36;
        max_n  = 1;

        /* for i = 1 we set pow4i = 4^1, pow9i = 9^1 so that in the loop
           starting at i = 2 we can multiply once to obtain 4^2 and 9^2. */
        pow4i = (seq_t)4;
        pow9i = (seq_t)9;

        if (n <= 1)
            return cache[n];
    }

    /* return cached value if already computed */
    if (n <= max_n)
        return cache[n];

    /* resize cache if necessary */
    if ((size_t)(n + 1) > (size_t)max_n + 1) {
        void *tmp = realloc(cache,
                            (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp)                /* allocation failure – return 0 as fallback */
            return (seq_t)0;
        cache = (seq_t *)tmp;
    }

    /* compute terms from max_n+1 up to n inclusive */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* pow4i and pow9i currently hold 4^(i-1) and 9^(i-1).
           Multiply to advance to 4^i and 9^i. */
        pow4i *= (seq_t)4;
        pow9i *= (seq_t)9;

        /* compute (9^(i+1) - 4^(i+1))/5
           = ( 9 * 9^i  - 4 * 4^i ) / 5  */
        seq_t t9 = pow9i * (seq_t)9;
        seq_t t4 = pow4i * (seq_t)4;
        seq_t term = (t9 - t4) / (seq_t)5;

        cache[i] = 23 * cache[i - 1] - 132 * cache[i - 2] + term;
    }

    max_n = n;
    return cache[n];
}
