/*
 * A230928  Number of black-square subarrays of (n+2) X (1+2) 0..3 arrays
 *          x(i,j) with each element diagonally or antidiagonally next to at
 *          least one element with value (x(i,j)+1) mod 4, no adjacent
 *          elements equal, and upper left element zero.
 * https://oeis.org/A230928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-2) + 5*a(n-4) for n ≥ 5, with a(1) = 2, a(2) = 2,
 *            a(3) = 8, a(4) = 8
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache (memoisation) of previously computed values.
       cache[0] is unused so that the index matches the OEIS offset. */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;        /* allocated capacity (≥ n+1)   */
    static idx_t   top    = 0;        /* highest index that has been computed */

    /* Ensure the cache can hold the requested index. */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;   /* keep index 0 unused          */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { abort(); }           /* allocation failure            */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any new terms that are required. */
    for (idx_t i = top + 1; i <= n; ++i) {
        if (i == 1 || i == 2) {
            cache[i] = (seq_t)2;
        } else if (i == 3 || i == 4) {
            cache[i] = (seq_t)8;
        } else {
            /* Apply the recurrence.  The ordinary arithmetic operators
               work for built‑in integral types and for the arbitrary‑
               precision types provided by the build. */
            cache[i] = 4 * cache[i-2] + 5 * cache[i-4];
        }
    }
    top = n;

    return cache[n];
}
