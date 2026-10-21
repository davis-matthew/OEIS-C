/*
 * A231105  Number of n X 5 0..3 arrays x(i,j) with each element horizontally
 *          or antidiagonally next to at least one element with value
 *          (x(i,j)+1) mod 4, no adjacent elements equal, and upper left
 *          element zero.
 * https://oeis.org/A231105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the empirical recurrence a(n) = 14*a(n-1) +
 *            35*a(n-2) - 4*a(n-3) + 4*a(n-4) + 32*a(n-5) - 32*a(n-6) for n ≥
 *            7 with initial values a(1)=0, a(2)=20, a(3)=286, a(4)=4674,
 *            a(5)=75346, a(6)=1217338. The implementation memoises the
 *            sequence so that calls may be in any order
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;        /* highest n for which cache has values */

    if (n == 0)                /* safety: sequence is defined only for n >= 1 */
        return (seq_t)0;

    if (n > max_n) {           /* need to extend cache to at least n */
        size_t new_size = (size_t)(n + 1);   /* indices 0..n, we leave 0 unused */

        /* Allocate or resize cache.  Cast the result of realloc so it compiles in C++. */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        /* Initialise base cases if this is the first allocation or if the array was
           previously smaller than the required base size.  The assignments are idempotent. */
        if (n >= 1) cache[1] = (seq_t)0;
        if (n >= 2) cache[2] = (seq_t)20;
        if (n >= 3) cache[3] = (seq_t)286;
        if (n >= 4) cache[4] = (seq_t)4674;
        if (n >= 5) cache[5] = (seq_t)75346;
        if (n >= 6) cache[6] = (seq_t)1217338;

        /* Compute any terms that have not yet been filled, starting from max_n+1 up to n. */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* For i <= 6 we already have the value from the base assignments above. */
            if (i <= 6) continue;

            seq_t term = (seq_t)14 * cache[i-1]
                       + (seq_t)35 * cache[i-2]
                       - (seq_t)4  * cache[i-3]
                       + (seq_t)4  * cache[i-4]
                       + (seq_t)32 * cache[i-5]
                       - (seq_t)32 * cache[i-6];

            cache[i] = term;
        }

        max_n = n;
    }

    return cache[n];
}
