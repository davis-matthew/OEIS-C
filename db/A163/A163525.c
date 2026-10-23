/*
 * A163525  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(0)=1, a(1)=25, a(2)=600, a(3)=14400, a(4)=345600*/
/*   a(5)=23*a(4)+23*a(3)+23*a(2)+23*a(1)-276*a(0)+1=8294100*/
/*   a(n)=23*a(n-1)+23*a(n-2)+23*a(n-3)+23*a(n-4)-276*a(n-5)  for n≥6 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;       /* array of computed a(i) */
    static size_t  cap   = 0;         /* current allocation size */
    static size_t  last  = 0;         /* largest index computed so far (last + 1) */

    /* Ensure the cache is large enough to hold indices up to n */
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? cap : 8;                 /* start with a small size */
        while (new_cap <= (size_t)n) new_cap <<= 1;     /* grow until big enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)   /* allocation failure – return 0 as a sensible fallback */
            return (seq_t)0;

        /* Initialize any newly allocated entries to 0 */
        if (new_cap > cap)
            for (size_t i = cap; i < new_cap; ++i)
                tmp[i] = (seq_t)0;

        cache = tmp;
        cap   = new_cap;
    }

    /* Compute values up to n if they are not already cached */
    for (size_t i = last; i <= (size_t)n; ++i) {
        if (i == 0)                    cache[i] = (seq_t)1;
        else if (i == 1)               cache[i] = (seq_t)25;
        else if (i == 2)               cache[i] = (seq_t)600;
        else if (i == 3)               cache[i] = (seq_t)14400;
        else if (i == 4)               cache[i] = (seq_t)345600;
        else if (i == 5) {
            /* a(5) = 23*a(4)+23*a(3)+23*a(2)+23*a(1)-276*a(0)+1 */
            seq_t val = 23 * cache[4] + 23 * cache[3]
                       + 23 * cache[2] + 23 * cache[1]
                       - 276 * cache[0] + (seq_t)1;
            cache[i] = val;
        }
        else {
            /* a(n) = 23*a(n-1)+23*a(n-2)+23*a(n-3)+23*a(n-4)-276*a(n-5) */
            seq_t val = 23 * cache[i-1] + 23 * cache[i-2]
                       + 23 * cache[i-3] + 23 * cache[i-4]
                       - 276 * cache[i-5];
            cache[i] = val;
        }
    }

    last = (size_t)n + 1;   /* we have computed up to n */
    return cache[(size_t)n];
}
