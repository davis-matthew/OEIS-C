/*
 * A130515  In triangular peg solitaire, number of distinct feasible pairs
 *          starting with one peg missing and finishing with one peg.
 * https://oeis.org/A130515
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-3) - a(n-6) - 5*a(n-9) + 5*a(n-12) + a(n-15) -
 *            3*a(n-18) + a(n-21)
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for the OEIS sequence A130515.  */
seq_t a(idx_t n)
{
    /* Only defined for n >= 2.  Return 0 for smaller indices. */
    if (n < 2)
        return (seq_t)0;

    /* Static cache for memoization.  cache[i] holds a(i).  */
    static seq_t *cache = NULL;   /* Dynamic array holding computed terms.  */
    static idx_t max_n = 1;       /* Highest index stored currently.  */

    /* Initialise cache with the first 21 known values (indices 2..22).  */
    if (cache == NULL)
    {
        /* Allocate space for indices 0..22 (size = 23).  */
        size_t init_size = 23;
        cache = (seq_t *)calloc(init_size, sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;       /* Allocation failed – return 0 as safety.  */

        /* Fill the known terms.  */
        cache[2]  = (seq_t)1;
        cache[3]  = (seq_t)4;
        cache[4]  = (seq_t)3;
        cache[5]  = (seq_t)17;
        cache[6]  = (seq_t)29;
        cache[7]  = (seq_t)27;
        cache[8]  = (seq_t)80;
        cache[9]  = (seq_t)125;
        cache[10] = (seq_t)108;
        cache[11] = (seq_t)260;
        cache[12] = (seq_t)356;
        cache[13] = (seq_t)300;
        cache[14] = (seq_t)637;
        cache[15] = (seq_t)832;
        cache[16] = (seq_t)675;
        cache[17] = (seq_t)1341;
        cache[18] = (seq_t)1665;
        cache[19] = (seq_t)1323;
        cache[20] = (seq_t)2500;
        cache[21] = (seq_t)3025;
        cache[22] = (seq_t)2352;

        max_n = 22;
    }

    /* Expand cache if necessary.  */
    if (n > max_n)
    {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;  /* Reallocation failed.  */
        cache = tmp;

        /* Compute missing terms up to n.  */
        for (idx_t i = max_n + 1; i <= n; ++i)
        {
            /* Compute a(i) by the recurrence.  Use the form
               left - right to avoid unsigned underflow.  */
            seq_t left  = 3 * cache[i - 3] + 5 * cache[i - 12] + cache[i - 15] + cache[i - 21];
            seq_t right =       cache[i - 6] + 5 * cache[i - 9]  + 3 * cache[i - 18];
            cache[i] = left - right;
        }

        max_n = n;
    }

    return cache[n];
}
