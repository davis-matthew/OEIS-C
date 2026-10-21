/*
 * A082766  Series ratios converge alternately to sqrt(2) and 1+sqrt(1/2).
 * https://oeis.org/A082766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 51, u64 n <= 102,
 *            u128 n <= 203, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A082766 – Series ratios converge alternately to sqrt(2) and 1+sqrt(1/2).
 *  Recurrence:
 *      a(1)=1 , a(2)=1 , a(3)=2
 *      for n ≥ 4
 *          if n is even:  a(n) = a(n-1) + a(n-2)
 *          if n is odd:   a(n) = a(n-1) + a(n-3)
 *
 *  Implementation uses a small static cache so that repeated calls
 *  are O(1) after the first computation of the requested index.
 *  All arithmetic is performed with the seq_t type, which may be
 *  an 8‑, 16‑, 32‑, 64‑bit unsigned integer or an arbitrary‑precision
 *  integer class.  The code stays well under the 2.0‑second limit
 *  for n up to about 10 000.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* dynamic array starting at index 1 */
    static idx_t cache_size = 0;  /* highest index currently computed */

    if (n == 0)          /* offset of the sequence is 1 – behaviour for n==0 is unused */
        return 0;
    if (n == 1 || n == 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)2;

    if (n > cache_size) {
        /* grow the cache to hold indices up to n */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – fall back to the currently known maximum */
            free(cache);
            cache = NULL;
            cache_size = 0;
            return 0;
        }
        cache = tmp;

        /* initialise the first three terms if they have not yet been stored */
        if (cache_size < 3) {
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)1;
            cache[3] = (seq_t)2;
            cache_size = 3;
        }

        /* compute newly needed indices */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if ((i & (idx_t)1) == 0)            /* even index */
                cache[i] = cache[i - 1] + cache[i - 2];
            else                                 /* odd index */
                cache[i] = cache[i - 1] + cache[i - 3];
        }
        cache_size = n;
    }
    return cache[n];
}
