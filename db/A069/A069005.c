/*
 * A069005  Let M = 4 X 4 matrix with rows /1,1,1,1/1,1,1,0/1,1,0,0/1,0,0,0/
 *          and A(n) = vector (x(n),y(n),z(n),t(n)) = M^n*A where A is the
 *          vector (1,1,1,1); then a(n)=z(n).
 * https://oeis.org/A069005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=7, a(3)=19, a(4)=56, a(5)=160, and for n≥6
 *            a(n)=2*a(n-1)+3*a(n-2)-a(n-3)-a(n-4). (OEIS A069005) Offset: 1
 *            (first term a(1)=1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary‑order calls */
    static seq_t *cache = NULL;   /* cache[0] holds a(1) */
    static idx_t cached_n = 0;    /* number of terms stored (>=0) */

    /* if we already have the value, return it immediately */
    if (n <= cached_n)
        return cache[n - 1];

    /* grow the cache to hold terms up to n */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)n * sizeof(seq_t));
        cached_n = 0;
    } else if (n > cached_n) {
        cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    }

    /* compute missing terms */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)
            val = (seq_t)1;
        else if (i == 2)
            val = (seq_t)7;
        else if (i == 3)
            val = (seq_t)19;
        else if (i == 4)
            val = (seq_t)56;
        else if (i == 5)
            val = (seq_t)160;
        else {
            /* recurrence: a(n)=2*a(n-1)+3*a(n-2)-a(n-3)-a(n-4) */
            val  = (seq_t)2 * cache[i - 2];
            val += (seq_t)3 * cache[i - 3];
            val -= cache[i - 4];
            val -= cache[i - 5];
        }
        cache[i - 1] = val;
    }

    cached_n = n;
    return cache[n - 1];
}
