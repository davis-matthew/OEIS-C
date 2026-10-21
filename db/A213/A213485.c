/*
 * A213485  Number of (w,x,y) with all terms in {0,...,n} and
 *          |w-x|+|x-y|+|y-w| != w+x+y.
 * https://oeis.org/A213485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)-7*a(n-2)+8*a(n-3)-7*a(n-4)+4*a(n-5)-a(n-6)
 *            a(0)=0, a(1)=4, a(2)=20, a(3)=54, a(4)=109, a(5)=191 (The
 *            sequence is OEIS A213485.)
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t last = 0;          /* highest index already computed */

    /* initialise cache with the first six terms if not done yet */
    if (!cache) {
        cache = (seq_t*)malloc(6 * sizeof(seq_t));
        if (!cache) return 0;      /* failure to allocate – unlikely */
        cache[0] = 0;
        cache[1] = 4;
        cache[2] = 20;
        cache[3] = 54;
        cache[4] = 109;
        cache[5] = 191;
        last = 5;
    }

    /* if the requested term is already in the cache, just return it */
    if (n <= last)
        return cache[n];

    /* grow the cache to hold up to n */
    idx_t newsize = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
    if (!tmp) return 0;          /* memory allocation failed */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term0 = cache[i-1];
        seq_t term1 = cache[i-2];
        seq_t term2 = cache[i-3];
        seq_t term3 = cache[i-4];
        seq_t term4 = cache[i-5];
        seq_t term5 = cache[i-6];

        /* avoid negative intermediate values: positive part and negative part */
        seq_t pos = 4 * term0 + 8 * term2 + 4 * term4;
        seq_t neg = 7 * term1 + 7 * term3 + term5;
        cache[i] = pos - neg;
    }

    last = n;
    return cache[n];
}
