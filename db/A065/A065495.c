/*
 * A065495  Number of (binary) bit strings of length n in which an odd length
 *          block of 0's is followed by an odd length block of 1's.
 * https://oeis.org/A065495
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence A065495 – number of binary strings of length n in which an odd
   block of 0's is followed by an odd block of 1's.
   Recurrence (derived from the generating function x^2/((1-2x)(1-2x^2-2x^3))):
         a(0) = 0
         a(1) = 0
         a(2) = 1
         a(n) = 2*a(n-1) + 2*a(n-2) - 2*a(n-3) - 4*a(n-4)   for n >= 3
   The offset is 2, i.e. a(2) is the first listed term. */

seq_t a(idx_t n)
{
    /* cache of already computed values */
    static seq_t *cache   = NULL;
    static idx_t  cache_len   = 0;   /* allocated length of cache   */
    static idx_t  max_computed = 0;   /* highest index that has a value */

    /* ensure the cache can hold indices up to n */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) abort();               /* allocation failed */
        /* initialise the newly added part to 0 */
        for (idx_t i = cache_len; i < new_len; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_len = new_len;
    }

    /* initialise the first terms if this is the first call */
    if (max_computed == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        max_computed = 2;
    }

    /* compute any missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* the recurrence uses a(i-1), a(i-2), a(i-3) and a(i-4)   */
        seq_t sum = 2 * cache[i-1];
        sum += 2 * cache[i-2];

        seq_t sub = 2 * cache[i-3];
        if (i >= 4)
            sub += 4 * cache[i-4];

        /* subtraction cannot underflow because the mathematical
           recurrence guarantees the result is non‑negative. */
        cache[i] = sum - sub;
    }
    if (n > max_computed) max_computed = n;

    /* indices below the offset are irrelevant – return 0 */
    if (n < 2)
        return (seq_t)0;

    return cache[n];
}
