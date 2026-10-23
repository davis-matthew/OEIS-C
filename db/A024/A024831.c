/*
 * A024831  a(n) = least m such that if r and s in {F(h)/F(2*h): h =
 *          1,2,...,n} satisfy r < s, then r < k/m < s for some integer k,
 *          where F = A000045 (Fibonacci numbers).
 * https://oeis.org/A024831
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(2)=2, a(3)=7, a(4)=10, a(5)=10, a(6)=15, a(7)=23,
 *            a(n)=a(n-1)+a(n-2)-1 for n>=8
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Memoisation is used to allow arbitrary order calls. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Behaviour for indices below the offset is irrelevant. */
    if (n < 2)
        return (seq_t)0;

    static seq_t *cache   = NULL;   /* memoization table */
    static size_t cache_cap = 0;    /* allocated capacity of cache */
    static idx_t last_computed = 1; /* highest index that has a value */

    /* Grow the cache if necessary. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 2)
            cache[2] = (seq_t)2;
        else if (i == 3)
            cache[3] = (seq_t)7;
        else if (i == 4)
            cache[4] = (seq_t)10;
        else if (i == 5)
            cache[5] = (seq_t)10;
        else if (i == 6)
            cache[6] = (seq_t)15;
        else if (i == 7)
            cache[7] = (seq_t)23;
        else /* i >= 8 */
            cache[i] = cache[i-1] + cache[i-2] - (seq_t)1;
    }
    last_computed = n;
    return cache[n];
}
