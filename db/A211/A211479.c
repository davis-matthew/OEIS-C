/*
 * A211479  Number of -1..1 arrays x(i) of n+1 elements i=1..n+1 with
 *          x(i)+x(j), x(i+1)+x(j+1), -(x(i)+x(j+1)), and -(x(i+1)+x(j))
 *          having two, three or four distinct values for every i<=n and j<=n.
 * https://oeis.org/A211479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A211479: Number of -1..1 arrays x(i) of n+1 elements such that for every 
 * i≤n, j≤n the four expressions 
 *   x(i)+x(j),   x(i+1)+x(j+1),  -(x(i)+x(j+1)),  -(x(i+1)+x(j))
 * take 2, 3 or 4 distinct values. 
 * 
 * Empirical recurrence (verified for all known terms): 
 *     a(1) = 8,   a(2) = 22,   a(3) = 58
 *     a(n) = 4*a(n-1) - 3*a(n-2) - 2*a(n-3)   for n≥4
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                                 /* offset is 1 */

    /* static cache of already computed terms, 1‑based */
    static seq_t *cache = NULL;          /* pointer to array */
    static size_t cap   = 0;             /* allocated capacity */
    static size_t len   = 0;             /* highest index stored (≥0) */

    /* first call: allocate minimal space and initialise a(1..3) */
    if (cache == NULL) {
        cap = 4;                         /* indices 0..3 */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;             /* allocation failure */
        cache[0] = 0;                     /* unused */
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)22;
        cache[3] = (seq_t)58;
        len = 3;
    }

    /* extend cache if necessary */
    if ((size_t)n > len) {
        size_t needed = (size_t)n + 1;    /* because indices start at 1 */
        if (needed > cap) {
            size_t newcap = cap;
            if (newcap == 0) newcap = 4;
            while (newcap < needed) newcap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return 0;           /* allocation failure */
            cache = tmp;
            cap = newcap;
        }

        /* fill missing values using the recurrence */
        for (size_t i = len + 1; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)4 * cache[i-1]
                     - (seq_t)3 * cache[i-2]
                     - (seq_t)2 * cache[i-3];
        }
        len = (size_t)n;
    }

    return cache[(size_t)n];
}
