/*
 * A222384  Sum of neighbor maps: number of nX5 binary arrays indicating the
 *          locations of corresponding elements equal to the sum mod 3 of
 *          their horizontal and antidiagonal neighbors in a random 0..2 nX5
 *          array.
 * https://oeis.org/A222384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 32*a(n-1) +256*a(n-3) -8192*a(n-4) -16384*a(n-6) +524288*a(n-7) for n>9
   Initial values: a(1)=24, a(2)=1024, a(3)=32768, a(4)=1048576,
                    a(5)=33546240, a(6)=1073741824, a(7)=34359738368,
                    a(8)=1099510054912, a(9)=35184372088832 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with the nine initial terms */
    if (cache == NULL) {
        cache_size = 10;                     /* indices 0..9 (0 unused) */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = 0;                        /* unused slot at index 0   */
        cache[1] = (seq_t)24;
        cache[2] = (seq_t)1024;
        cache[3] = (seq_t)32768;
        cache[4] = (seq_t)1048576;
        /* values fit 64‑bit */
        cache[5] = (seq_t)33546240;
        cache[6] = (seq_t)1073741824;
        cache[7] = (seq_t)34359738368ULL;
        cache[8] = (seq_t)1099510054912ULL;
        cache[9] = (seq_t)35184372088832ULL;
    }

    /* if n is already in cache, just return it */
    if (n < cache_size) return cache[n];

    /* grow cache to size n+1 */
    void *tmp = realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = (seq_t*)tmp;

    /* compute new values up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term = 0;
        term += (seq_t)32 * cache[i - 1];
        term += (seq_t)256 * cache[i - 3];
        term -= (seq_t)8192 * cache[i - 4];
        term -= (seq_t)16384 * cache[i - 6];
        term += (seq_t)524288 * cache[i - 7];
        cache[i] = term;
    }
    cache_size = n + 1;
    return cache[n];
}
