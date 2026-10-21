/*
 * A215192  Number of arrays of 5 0..n integers with no sum of consecutive
 *          elements equal to a disjoint adjacent sum of an equal number of
 *          elements.
 * https://oeis.org/A215192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 4*a(n-3) + 2*a(n-4) + 2*a(n-5) +
 *            2*a(n-6) - 4*a(n-7) - a(n-8) + 3*a(n-9) - a(n-10) Initial
 *            values: a(1)=0, a(2)=30, a(3)=216, a(4)=954, a(5)=2940,
 *            a(6)=7404, a(7)=16092, a(8)=31560, a(9)=57072, a(10)=96990
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cached_max = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* guard for accidental 0 */

    /* initialise memoisation storage on first call */
    if (cache == NULL) {
        cache_size = 11;                      /* indices 0 .. 10 */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;                         /* unused */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)30;
        cache[3] = (seq_t)216;
        cache[4] = (seq_t)954;
        cache[5] = (seq_t)2940;
        cache[6] = (seq_t)7404;
        cache[7] = (seq_t)16092;
        cache[8] = (seq_t)31560;
        cache[9] = (seq_t)57072;
        cache[10] = (seq_t)96990;
        cached_max = 10;
    }

    if (n <= cached_max) return cache[n];

    /* ensure enough room for n + 1 entries */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms with the linear recurrence */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t val = (seq_t)3 * cache[i-1] - cache[i-2]
                    - (seq_t)4 * cache[i-3]
                    + (seq_t)2 * cache[i-4]
                    + (seq_t)2 * cache[i-5]
                    + (seq_t)2 * cache[i-6]
                    - (seq_t)4 * cache[i-7]
                    - cache[i-8]
                    + (seq_t)3 * cache[i-9]
                    - cache[i-10];
        cache[i] = val;
    }

    cached_max = n;
    return cache[n];
}
