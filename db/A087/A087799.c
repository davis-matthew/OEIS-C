/*
 * A087799  a(n) = 10*a(n-1) - a(n-2), starting with a(0) = 2 and a(1) = 10.
 * https://oeis.org/A087799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) - a(n-2); a(0)=2, a(1)=10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store computed values */
    static seq_t *cache = NULL;
    static size_t cap  = 0;   /* allocated capacity of cache */
    static idx_t  max   = 0;  /* highest index that has been computed */

    /* base cases */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)10;

    /* initialise cache once */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)10;
        max   = 1;                            /* we have a(0) and a(1) */
    }

    /* grow the array if n is outside current capacity */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 1;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* reallocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute all missing terms up to n */
    if (max < n) {
        for (idx_t i = max + 1; i <= n; ++i) {
            seq_t val = cache[i - 1];
            val = val * (seq_t)10;    /* 10 * a(i-1) */
            val = val - cache[i - 2]; /* minus a(i-2) */
            cache[i] = val;
        }
        max = n;
    }

    return cache[(size_t)n];
}
