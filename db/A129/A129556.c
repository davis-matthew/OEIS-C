/*
 * A129556  Numbers k such that the k-th centered pentagonal number A005891(k)
 *          = (5k^2 + 5k + 2)/2 is a square.
 * https://oeis.org/A129556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=2, a(3)=21, a(4)=95; for n>=5, a(n) = 38*a(n-2) -
 *            a(n-4) + 18
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for indices < 1, return 0 */
        return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  max_n  = 0;   /* largest n already computed (0 means none) */
    static size_t cap    = 0;   /* allocated capacity of cache (elements) */

    /* Grow cache if necessary (allocate at least n elements) */
    if (n > cap) {
        size_t newcap = n > cap ? n : cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();                  /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)
            val = (seq_t)0;
        else if (i == 2)
            val = (seq_t)2;
        else if (i == 3)
            val = (seq_t)21;
        else if (i == 4)
            val = (seq_t)95;
        else
            /* a(i) = 38*a(i-2) - a(i-4) + 18  */
            val = ((seq_t)38) * cache[(i - 3)] - cache[(i - 5)] + (seq_t)18;

        cache[i - 1] = val;
    }

    max_n = n;
    return cache[n - 1];
}
