/*
 * A088132  a(n) equals the square of the n-th partial sum added to twice the
 *          n-th partial sum of the squares, divided by a(n-1), for all n>1,
 *          with a(0)=1, a(1)=3.
 * https://oeis.org/A088132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-3) for n > 3; a(0)=1, a(1)=3, a(2)=12,
 *            a(3)=47
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of already computed terms */
    static seq_t *cache = NULL;        /* array holding terms a(0) ... a(max)   */
    static idx_t cache_len = 0;        /* number of terms stored in cache       */
    static idx_t cache_alloc = 0;      /* allocated size of cache              */

    /* initialise cache with the first four terms */
    if (cache_len == 0) {
        cache_alloc = 4;
        cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure, should not occur */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)47;
        cache_len = 4;
    }

    /* compute terms up to the requested index */
    while (cache_len <= n) {
        /* recurrence: a(k) = 4*a(k-1) - a(k-3)               */
        seq_t next = 4 * cache[cache_len - 1] - cache[cache_len - 3];

        /* enlarge storage if needed */
        if (cache_len == cache_alloc) {
            cache_alloc <<= 1;                     /* double the capacity      */
            seq_t *tmp = (seq_t *)realloc(cache,
                                           cache_alloc * sizeof(seq_t));
            if (!tmp) return 0;                   /* allocation failure       */
            cache = tmp;
        }

        cache[cache_len++] = next;
    }

    return cache[n];
}
