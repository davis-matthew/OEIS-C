/*
 * A221513  Number of 0..6 arrays of length n with each element differing from
 *          at least one neighbor by 2 or more, starting with 0.
 * https://oeis.org/A221513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 14*a(n-2) + 29*a(n-3) + 28*a(n-4) + a(n-5) +
 *            27*a(n-6) + 8*a(n-7) + 2*a(n-8); Initial values: a(1)=0, a(2)=5,
 *            a(3)=21, a(4)=150, a(5)=884, a(6)=5513, a(7)=33860, a(8)=208756
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(1) is the first defined term; for n < 1 return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Static cache for computed values.  cache[0] is unused. */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static idx_t   max_n = 0;   /* largest n for which cache[n] is valid */

    /* Allocate or grow the cache if necessary. */
    if (!cache || (size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 9)          /* at least indices 1..8 */
            new_cap = 9;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first eight terms on the first call. */
    if (max_n == 0) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)21;
        cache[4] = (seq_t)150;
        cache[5] = (seq_t)884;
        cache[6] = (seq_t)5513;
        cache[7] = (seq_t)33860;
        cache[8] = (seq_t)208756;
        max_n   = 8;
    }

    if (n <= max_n)
        return cache[n];

    /* Compute forward using the linear recurrence. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val = 0;
        val += (seq_t)3  * cache[i-1];
        val += (seq_t)14 * cache[i-2];
        val += (seq_t)29 * cache[i-3];
        val += (seq_t)28 * cache[i-4];
        val +=        cache[i-5];
        val += (seq_t)27 * cache[i-6];
        val += (seq_t)8  * cache[i-7];
        val += (seq_t)2  * cache[i-8];
        cache[i] = val;
    }

    max_n = n;
    return cache[n];
}
