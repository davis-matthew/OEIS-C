/*
 * A141072  Sum of diagonal numbers in a Pascal-like triangle with index of
 *          asymmetry y = 3 and index of obliquity z = 0 (going upwards, left
 *          to right).
 * https://oeis.org/A141072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A141072: Sum of diagonal numbers in a Pascal‑like triangle
   with index of asymmetry y = 3 and index of obliquity z = 0.
   Recurrence (valid for n ≥ 6):
       a(n) = -(3 + (-1)^n)/2 + a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)
       a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=6
   This function memoises previously computed terms and works for any
   n ≥ 1, independently of call order.
   The computation uses only integer arithmetic on seq_t. */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* cache[1] … cache[cache_size] store a(1)…a(cache_size)   */
    static seq_t *cache = NULL;          /* 1‑based indexing, cache[0] unused */
    static idx_t cache_size = 0;         /* largest index stored so far    */

    if (n == 0)            /* offset is 1, undefined for 0 */
        return (seq_t)0;    /* harmless placeholder */

    /* grow the cache if necessary */
    if (cache_size < n)
    {
        idx_t new_size = n;                     /* need indices up to n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (tmp == NULL)
            abort();                            /* allocation failed */
        cache = tmp;

        /* initialise the first five terms if not yet done */
        if (cache_size < 5)
        {
            if (new_size >= 1) cache[1] = (seq_t)1;
            if (new_size >= 2) cache[2] = (seq_t)1;
            if (new_size >= 3) cache[3] = (seq_t)2;
            if (new_size >= 4) cache[4] = (seq_t)3;
            if (new_size >= 5) cache[5] = (seq_t)6;
        }

        /* compute new terms using the recurrence */
        for (idx_t i = cache_size + 1; i <= new_size; ++i)
        {
            if (i <= 5)          /* already initialised */
                continue;

            seq_t sum = cache[i - 1] + cache[i - 2] + cache[i - 3]
                      + cache[i - 4] + cache[i - 5];

            /* sub = (i even ? 2 : 1)  because  -(3+(-1)^i)/2  */
            seq_t sub = ( (i & 1ULL) == 0ULL ) ? (seq_t)2 : (seq_t)1;

            cache[i] = sum - sub;
        }

        cache_size = new_size;
    }

    return cache[n];
}
