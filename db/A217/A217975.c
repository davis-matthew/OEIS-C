/*
 * A217975  Integers k such that 2*k^2 - 7 is a square.
 * https://oeis.org/A217975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) - a(n-4) with a(1)=2, a(2)=4, a(3)=8, a(4)=22
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the nth term of OEIS sequence A217975.
   The sequence satisfies the recurrence:
       a(n) = 6*a(n-2) - a(n-4)   for n >= 5
   with initial values a(1)=2, a(2)=4, a(3)=8, a(4)=22.
   The function memoizes previously computed terms to support
   arbitrary call order.  */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* index 1..cache_size will be valid */
    static idx_t cache_size = 0;  /* number of terms currently stored */

    if (n == 0) return 0;  /* out of range, but safe guard */

    /* Ensure cache can hold n */
    if (cache == NULL) {
        /* Allocate space for indices 0..n */
        cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!cache) abort();   /* allocation failure */
        cache[0] = 0; /* unused */
    } else if (n > cache_size) {
        /* Grow the cache */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failure */
        cache = tmp;
    }

    /* Compute missing terms */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1)          cache[i] = (seq_t)2;
        else if (i == 2)     cache[i] = (seq_t)4;
        else if (i == 3)     cache[i] = (seq_t)8;
        else if (i == 4)     cache[i] = (seq_t)22;
        else                 cache[i] = 6 * cache[i - 2] - cache[i - 4];
    }

    cache_size = n;
    return cache[n];
}
