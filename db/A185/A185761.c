/*
 * A185761  Number of (n+1) X 2 binary arrays with no 2 X 2 subblock trace
 *          equal to any horizontal or vertical neighbor 2 X 2 subblock trace.
 * https://oeis.org/A185761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A185761: a(n) = a(n-1) + 4*a(n-2); a(1)=16, a(2)=40
   Computes the sequence values with dynamic recurrence and memoisation.
   Works for any seq_t/idx_t integer type supplied by the build.
   The function may be called in any order of indices.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Values for n < 1 are undefined; return 0 */
    if (n < 1) return (seq_t)0;

    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;     /* highest index currently stored */

    /* Ensure the cache can hold index n (and a[0] unused) */
    if (cap < n)
    {
        /* Allocate new array, preserving existing values */
        seq_t *new_arr = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);

        if (cache)
        {
            /* Copy old contents */
            for (idx_t i = 1; i <= cap; ++i) new_arr[i] = cache[i];
            free(cache);
        }

        cache = new_arr;
        cap   = n;

        /* Initialise base terms if the table is large enough */
        if (cap >= 1) cache[1] = (seq_t)16;
        if (cap >= 2) cache[2] = (seq_t)40;
    }

    /* Base cases */
    if (n == 1) return cache[1];
    if (n == 2) return cache[2];

    /* Compute missing values up to n */
    for (idx_t k = 3; k <= n; ++k)
        cache[k] = cache[k - 1] + cache[k - 2] * (seq_t)4;

    return cache[n];
}
