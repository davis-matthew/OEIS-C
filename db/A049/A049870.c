/*
 * A049870  a(n) = Sum{a(k): k=0,1,2,...,n-4,n-2,n-1}; a(n-3) is not a
 *          summand; initial terms are 1,2,4.
 * https://oeis.org/A049870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Sum{ a(k) : k = 0,1,2,…,n-4,n-2,n-1 } (a(n-3) is omitted)
 * Recurrence (for n ≥ 4): a(n) = 2*a(n-1) - a(n-3) + a(n-4)
 * Initial values: a(0)=1, a(1)=2, a(2)=4, a(3)=6
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* capacity (number of allocated elements)   */
    static idx_t  len   = 0;   /* number of elements already computed (next free index) */

    /* Grow the cache if necessary (ensure it holds indices 0 … n) */
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 4;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);   /* allocation failed */
        cap = new_cap;
    }

    /* Initialise the first four entries on the first call */
    if (len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)6;
        len = 4;
    }

    /* Compute missing values up to n (in increasing order) */
    if (n >= len) {
        for (idx_t i = len; i <= n; ++i) {
            /* For i ≥ 4 use the recurrence */
            cache[i] = ((seq_t)2 * cache[i - 1]) - cache[i - 3] + cache[i - 4];
        }
        len = n + 1;
    }

    return cache[n];
}
