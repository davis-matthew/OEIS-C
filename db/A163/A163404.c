/*
 * A163404  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163404
   Recurrence: a(n) = 9*a(n-1)+9*a(n-2)+9*a(n-3)+9*a(n-4)-45*a(n-5) for n≥6
   with a(0)=1, a(1)=11, a(2)=110, a(3)=1100, a(4)=11000.
   Special value a(5) = 1 + 9*a(4)+9*a(3)+9*a(2)+9*a(1)-45*a(0) = 109945.
*/
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* pointer to array of seq_t */
    static idx_t cache_size = 0;  /* current number of cached terms */

    /* return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* enlarge cache to accommodate index n */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        abort();                /* memory allocation failed */
    cache = new_cache;

    /* initialise the first five elements (overwritten harmlessly if they
       already existed) */
    if (new_size > 0) cache[0] = (seq_t)1;
    if (new_size > 1) cache[1] = (seq_t)11;
    if (new_size > 2) cache[2] = (seq_t)110;
    if (new_size > 3) cache[3] = (seq_t)1100;
    if (new_size > 4) cache[4] = (seq_t)11000;

    /* compute a(5) only once, when it becomes needed */
    if (new_size > 5 && cache_size <= 5) {
        cache[5] =
            (seq_t)1
            + (seq_t)9 * cache[4]
            + (seq_t)9 * cache[3]
            + (seq_t)9 * cache[2]
            + (seq_t)9 * cache[1]
            - (seq_t)45 * cache[0];
    }

    /* compute remaining terms using the recurrence
       starting from the first index not yet computed */
    idx_t start = (cache_size < 6) ? 6 : cache_size;
    for (idx_t i = start; i < new_size; ++i) {
        cache[i] =
            (seq_t)9  * cache[i-1]
          + (seq_t)9  * cache[i-2]
          + (seq_t)9  * cache[i-3]
          + (seq_t)9  * cache[i-4]
          - (seq_t)45 * cache[i-5];
    }

    cache_size = new_size;
    return cache[n];
}
