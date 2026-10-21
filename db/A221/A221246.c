/*
 * A221246  Number of 2 X n arrays of occupancy after each element moves to
 *          some horizontal, diagonal or antidiagonal neighbor.
 * https://oeis.org/A221246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 54*a(n-3) + 36*a(n-4) for n>5, with initial
 *            values a(1)=0, a(2)=9, a(3)=50, a(4)=484, a(5)=3864
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* off‑the‑offset values are undefined */

    static seq_t *cache = NULL;   /* 1‑based array: cache[i] == a(i) */
    static idx_t cache_size = 0;  /* allocated size (indices 0 .. cache_size-1) */
    static idx_t max_computed = 0;/* largest n for which cache[n] is valid */

    /* Allocate or grow cache to hold indices up to n */
    if ((idx_t)cache_size <= n) {
        idx_t new_size = n + 1;   /* 1‑based indexing; ignore element 0 */
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        /* Zero‑initialize any newly added part */
        if (new_size > cache_size)
            memset(tmp + cache_size,
                   0,
                   (size_t)(new_size - cache_size) * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }

    /* Set the base values up to the smaller of n and 5 */
    idx_t upto = (n < 5 ? n : 5);
    if (upto >= 1) cache[1] = (seq_t)0;
    if (upto >= 2) cache[2] = (seq_t)9;
    if (upto >= 3) cache[3] = (seq_t)50;
    if (upto >= 4) cache[4] = (seq_t)484;
    if (upto >= 5) cache[5] = (seq_t)3864;
    if (max_computed < upto) max_computed = upto;

    /* Compute any missing terms using the recurrence */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* i is guaranteed >= 6 here */
            seq_t t1 = 9 * cache[i - 1];
            seq_t t2 = 54 * cache[i - 3];
            seq_t t3 = 36 * cache[i - 4];
            cache[i] = t1 - t2 + t3;
        }
        max_computed = n;
    }

    return cache[n];
}
