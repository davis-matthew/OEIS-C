/*
 * A184469  1/6 the number of (n+2) X 3 0..2 arrays with each 3 X 3 subblock
 *          containing one of one value, four of another, and four of the
 *          last.
 * https://oeis.org/A184469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            3*a(n-1)+96*a(n-3)-288*a(n-4)-2700*a(n-6)+8100*a(n-7)+23328*a(n-9)-69984*a(n-10)
 *            (empirical recurrence, valid for n ≥ 11; a(1) … a(10) are
 *            prescribed)
 * Range:     u8 none, u16 n <= 5, u32 n <= 13, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of already computed terms (index 0 unused) */
static seq_t *cache = NULL;
static idx_t cache_n = 0;          /* highest index stored in cache */

/* prescribed initial values a(1) … a(10) */
static const seq_t init_vals[10] = {
    315ULL,
    1095ULL,
    3705ULL,
    12339ULL,
    45585ULL,
    161739ULL,
    559305ULL,
    2147931ULL,
    7836561ULL,
    27667251ULL
};

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined below the offset */
        return (seq_t)0;

    /* initialise cache on first use */
    if (!cache) {
        size_t sz = 11;                 /* indices 0..10 (0 unused) */
        cache = (seq_t *)realloc(NULL, sz * sizeof(seq_t));
        cache[0] = (seq_t)0;           /* placeholder for index 0 */
        for (size_t i = 1; i <= 10; ++i)
            cache[i] = init_vals[i-1];
        cache_n = 10;
    }

    /* extend cache up to requested n */
    if (n > cache_n) {
        size_t new_size = (size_t)n + 1;       /* allocate indices 0..n */
        if (new_size > cache_n) {
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) {                       /* allocation failure: abort */
                /* In the unlikely event of a bad allocation, return 0 */
                cache = NULL;
                cache_n = 0;
                return (seq_t)0;
            }
            cache = tmp;
        }
        /* compute terms from cache_n+1 up to n sequentially */
        for (idx_t i = cache_n + 1; i <= n; ++i) {
            /* recurrence uses terms that already exist for i ≥ 11 */
            seq_t term = cache[i-1] * (seq_t)3
                       + cache[i-3] * (seq_t)96
                       - cache[i-4] * (seq_t)288
                       - cache[i-6] * (seq_t)2700
                       + cache[i-7] * (seq_t)8100
                       + cache[i-9] * (seq_t)23328
                       - cache[i-10] * (seq_t)69984;
            cache[i] = term;
        }
        cache_n = n;
    }

    return cache[n];
}
