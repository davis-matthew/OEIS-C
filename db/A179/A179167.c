/*
 * A179167  a(n) is the number of red balls in an urn such that if the urn
 *          contains b(n)=A101265(n) blue balls, the probability of drawing 3
 *          red balls without replacement is the same as drawing 1 red and 2
 *          blue balls without replacement; that is, binomial(a(n),3) =
 *          binomial(a(n),1) * binomial(b(n),2).
 * https://oeis.org/A179167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) + a(n-3) (for n>4) Initial values:
 *            a(1)=3, a(2)=4, a(3)=11, a(4)=37
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;             /* behaviour for n<1 does not matter */

    /* static cache for previously computed values */
    static seq_t *cache = NULL;      /* cache[i] holds a(i+1)  (zero‑based) */
    static idx_t len   = 0;          /* number of values stored */
    static idx_t cap   = 0;          /* allocated capacity */

    /* if we already have the requested value, return it */
    if (n <= len)
        return cache[n-1];

    /* grow the cache if necessary */
    if (cap == 0) {
        cap = 8;                            /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }
    while (cap < n) {                       /* need at least n elements */
        cap <<= 1;                          /* double the capacity */
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* compute missing terms */
    for (idx_t i = len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)          val = 3;
        else if (i == 2)     val = 4;
        else if (i == 3)     val = 11;
        else if (i == 4)     val = 37;
        else {
            /* recurrence: a(n) = 5*a(n-1) - 5*a(n-2) + a(n-3) */
            seq_t a1 = cache[i-2];   /* a(i-1)  */
            seq_t a2 = cache[i-3];   /* a(i-2)  */
            seq_t a3 = cache[i-4];   /* a(i-3)  */
            val = 5*a1 - 5*a2 + a3;
        }
        cache[i-1] = val;              /* store a(i) */
    }
    len = n;                            /* update length */
    return cache[n-1];
}
