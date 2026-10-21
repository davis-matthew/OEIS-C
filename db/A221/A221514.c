/*
 * A221514  Number of 0..7 arrays of length n with each element differing from
 *          at least one neighbor by 2 or more, starting with 0.
 * https://oeis.org/A221514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 21*a(n-2) + 58*a(n-3) + 79*a(n-4) + *
 *            32*a(n-5) + 23*a(n-6) + 4*a(n-7) + 8*a(n-8), * a(1)=0, a(2)=6,
 *            a(3)=31, a(4)=252, a(5)=1765, * a(6)=12872, a(7)=92934,
 *            a(8)=672526
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* undefined for n < 1, return 0 in that case */
    if (n < (idx_t)1) return (seq_t)0;

    /* Persistent cache of computed terms */
    static seq_t *cache = NULL;  /* array of a(k), 1-based index at cache[k-1] */
    static size_t cache_cap = 0; /* allocated length of the array */
    static idx_t  up_to  = 0;    /* highest index whose value is stored */

    /* Initialise cache on first call */
    if (!cache) {
        /* at least 8 terms are required for the recurrence */
        size_t init = 8;
        if ((size_t)n > init) init = (size_t)n;  /* ensure enough space for n */
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache) return (seq_t)0;             /* allocation failure */

        /* Seed the first 8 terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)31;
        cache[3] = (seq_t)252;
        cache[4] = (seq_t)1765;
        cache[5] = (seq_t)12872;
        cache[6] = (seq_t)92934;
        cache[7] = (seq_t)672526;

        cache_cap = init;
        up_to     = 8;
    }

    /* Grow the cache if the requested index exceeds the current capacity */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;          /* double size */
        seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_buf) return (seq_t)0;                      /* allocation failure */
        cache      = new_buf;
        cache_cap  = new_cap;
    }

    /* If the value is already known, return it immediately */
    if (n <= up_to) return cache[n-1];

    /* Compute missing terms up to n; recurrence needs 8 previous terms */
    for (idx_t i = up_to + 1; i <= n; ++i) {
        size_t idx = (size_t)(i - 1);  /* index of a(i) in 0‑based array */
        seq_t val  = (seq_t)0;
        val += (seq_t)3  * cache[idx - 1];
        val += (seq_t)21 * cache[idx - 2];
        val += (seq_t)58 * cache[idx - 3];
        val += (seq_t)79 * cache[idx - 4];
        val += (seq_t)32 * cache[idx - 5];
        val += (seq_t)23 * cache[idx - 6];
        val += (seq_t)4  * cache[idx - 7];
        val += (seq_t)8  * cache[idx - 8];
        cache[idx] = val;
    }
    up_to = n;
    return cache[n-1];
}
