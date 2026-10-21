/*
 * A094854  Number of (s(0), s(1), ..., s(2n)) such that 0 < s(i) < 9 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n, s(0) = 4, s(2n) = 4.
 * https://oeis.org/A094854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 15*a(n-2) + 10*a(n-3) - a(n-4) (n ≥ 4) initial
 *            values: a(0)=1, a(1)=2, a(2)=6, a(3)=20
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation uses a simple memoisation cache.
    It is valid in both C11 and C++17.  For an arbitrary‑precision 
    seq_t the arithmetic operators (+, -, *) are assumed to be overloaded. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* memoised values                */
    static idx_t    cache_len = 0;       /* number of cached terms (n+1)   */

    /* Allocate or grow the cache if needed */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;                    /* may be NULL if allocation fails */

        /* initialise successive terms */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0)                cache[i] = (seq_t)1;
            else if (i == 1)            cache[i] = (seq_t)2;
            else if (i == 2)            cache[i] = (seq_t)6;
            else if (i == 3)            cache[i] = (seq_t)20;
            else {
                seq_t t1 = cache[i-1] * (seq_t)7;
                seq_t t2 = cache[i-2] * (seq_t)15;
                seq_t t3 = cache[i-3] * (seq_t)10;
                seq_t t4 = cache[i-4];
                cache[i] = t1 - t2 + t3 - t4;
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
