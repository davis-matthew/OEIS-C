/*
 * A131075  First subdiagonal of triangular array T: T(j,1) = 1 for ((j-1) mod
 *          8) < 4, else 0; T(j,k) = T(j-1,k-1) + T(j,k-1) for 2 <= k <= j.
 * https://oeis.org/A131075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=4, a(4)=7, a(5)=11 a(n) = 6*a(n-1) -
 *            14*a(n-2) + 16*a(n-3) - 10*a(n-4) + 4*a(n-5) for n > 5
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    if (n == 0)   /* the sequence is offset 1; return 0 for safety */
        return (seq_t)0;

    /* Static cache: values 1 .. cache_max are stored in cache array. */
    static seq_t *cache = NULL;
    static idx_t  cache_cap  = 0;   /* allocated size (indices 0 .. cache_cap-1) */
    static idx_t  cache_max  = 0;   /* largest n whose value is cached */

    if (cache_cap == 0) {           /* first call: initialise cache */
        cache_cap = 6;              /* need indices 0..5 (0 unused)          */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;           /* not used                               */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)7;
        cache[5] = (seq_t)11;
        cache_max = 5;
    }

    if (n >= cache_cap) {           /* grow cache if we need more space   */
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values using the recurrence if necessary. */
    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t val = 6 * cache[i - 1];
            val -= 14 * cache[i - 2];
            val += 16 * cache[i - 3];
            val -= 10 * cache[i - 4];
            val += 4 * cache[i - 5];
            cache[i] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
