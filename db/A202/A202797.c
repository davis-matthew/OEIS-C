/*
 * A202797  Number of n X 3 binary arrays with every one adjacent to another
 *          one horizontally or vertically.
 * https://oeis.org/A202797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A202797: Number of n × 3 binary arrays with every 1 adjacent
 * horizontally or vertically.
 *
 * Formula (empirical recurrence):
 * a(n) = 7*a(n-1) - 3*a(n-2) + 19*a(n-3) - 3*a(n-4)
 *        + 25*a(n-5) + 24*a(n-6) - 22*a(n-7) - 16*a(n-8)
 *
 * Initial values (for n = 1 … 8):
 *  a(1) =   4
 *  a(2) =  36
 *  a(3) = 250
 *  a(4) = 1718
 *  a(5) =11988
 *  a(6) =83518
 *  a(7) =581518
 *  a(8) =4049700
 *
 * The function memoises all values that have been computed so it can be
 * called in any order.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) is unused; the sequence is defined for n ≥ 1 */
    if (n == 0) return (seq_t)0;

    /* static cache for memoisation (indices 0 … max_cached) */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;      /* allocated size (number of entries) */
    static idx_t  max_cached = 0;      /* largest n computed so far */

    /* initialise cache with space for indices 0 … 8 (0 unused) */
    if (cache == NULL) {
        cache_cap = 9;                         /* indices 0 … 8 */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();            /* allocation failure */

        /* set the first eight values explicitly */
        cache[0] = (seq_t)0;          /* unused */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)250;
        cache[4] = (seq_t)1718;
        cache[5] = (seq_t)11988;
        cache[6] = (seq_t)83518;
        cache[7] = (seq_t)581518;
        cache[8] = (seq_t)4049700;

        max_cached = 8;
    }

    /* if the required value is already in the cache, return it */
    if (n <= max_cached) return cache[n];

    /* grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;   /* double */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();                       /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute the missing terms using the recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t val = 0;

        val += (seq_t)7   * cache[i - 1];
        val -= (seq_t)3   * cache[i - 2];
        val += (seq_t)19  * cache[i - 3];
        val -= (seq_t)3   * cache[i - 4];
        val += (seq_t)25  * cache[i - 5];
        val += (seq_t)24  * cache[i - 6];
        val -= (seq_t)22  * cache[i - 7];
        val -= (seq_t)16  * cache[i - 8];

        cache[i] = val;
    }

    max_cached = n;
    return cache[n];
}
