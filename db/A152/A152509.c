/*
 * A152509  1/30 of the number of permutations of 5 indistinguishable copies
 *          of 1..n with exactly 2 local maxima.
 * https://oeis.org/A152509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 68*a(n-1) - 708*a(n-2) + 2016*a(n-3) for n>3 Initial
 *            values: a(1)=0, a(2)=2, a(3)=139
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    if (n == 0) return 0;          /* undefined for n<1 */

    /* initialise cache once */
    if (cache == NULL)
    {
        cache = (seq_t *)malloc(sizeof(seq_t) * 4); /* indices 0..3 */
        cache[0] = 0;                      /* unused */
        cache[1] = 0;                      /* a(1) */
        cache[2] = 2;                      /* a(2) */
        cache[3] = 139;                    /* a(3) */
        cap = 3;
    }

    /* return already computed value if possible */
    if (n <= cap) return cache[n];

    /* extend cache to reach n */
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    for (idx_t i = cap + 1; i <= n; ++i)
    {
        seq_t t1 = 68 * cache[i - 1];      /* 68*a(i-1) */
        seq_t t2 = 2016 * cache[i - 3];    /* 2016*a(i-3) */
        seq_t t3 = 708 * cache[i - 2];      /* 708*a(i-2)  */
        cache[i] = t1 + t2 - t3;           /* a(i) = 68*a(i-1)-708*a(i-2)+2016*a(i-3) */
    }
    cap = n;
    return cache[n];
}
