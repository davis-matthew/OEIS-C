/*
 * A211462  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          x(i)+x(j), x(i+1)+x(j+1), -(x(i)+x(j+1)), and -(x(i+1)+x(j))
 *          having one, two or three distinct values for every i<=n and j<=n.
 * https://oeis.org/A211462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - a(n-2) -55*a(n-3) +70*a(n-4) +193*a(n-5) *
 *            -357*a(n-6) -320*a(n-7) +802*a(n-8) +238*a(n-9) * -922*a(n-10)
 *            -39*a(n-11) +533*a(n-12) -29*a(n-13) * -136*a(n-14) +6*a(n-15)
 *            +12*a(n-16) * with a(1)=25, a(2)=65, a(3)=169, a(4)=407,
 *            a(5)=977, * a(6)=2283, a(7)=5313, a(8)=12259, a(9)=28...
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 23, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init_vals[16] = {
    (seq_t)25,     (seq_t)65,     (seq_t)169,    (seq_t)407,
    (seq_t)977,    (seq_t)2283,   (seq_t)5313,   (seq_t)12259,
    (seq_t)28253,  (seq_t)65023,  (seq_t)149817, (seq_t)345739,
    (seq_t)799821, (seq_t)1855467, (seq_t)4317293, (seq_t)10077003
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;   /* largest n for which cache is valid */
    static size_t cache_len = 0; /* allocated length of cache */

    /* initialise cache with the first 16 terms */
    if (cached_n == 0) {
        cache_len = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        for (size_t i = 0; i < 16; ++i)
            cache[i] = init_vals[i];
        cached_n = 16;
    }

    /* if already known return it */
    if (n <= cached_n)
        return cache[n - 1];

    /* grow cache if necessary */
    if (n > cache_len) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        cache_len = n;
    }

    /* compute missing terms up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        size_t idx = (size_t)(i - 2); /* a(i-1) */
        seq_t val =
            (seq_t)6   * cache[idx]                 /* a(i-1) */
          - cache[idx - 1]                           /* a(i-2) */
          - (seq_t)55 * cache[idx - 2]               /* a(i-3) */
          + (seq_t)70 * cache[idx - 3]               /* a(i-4) */
          + (seq_t)193 * cache[idx - 4]              /* a(i-5) */
          - (seq_t)357 * cache[idx - 5]              /* a(i-6) */
          - (seq_t)320 * cache[idx - 6]              /* a(i-7) */
          + (seq_t)802 * cache[idx - 7]              /* a(i-8) */
          + (seq_t)238 * cache[idx - 8]              /* a(i-9) */
          - (seq_t)922 * cache[idx - 9]              /* a(i-10) */
          - (seq_t)39  * cache[idx -10]              /* a(i-11) */
          + (seq_t)533 * cache[idx -11]              /* a(i-12) */
          - (seq_t)29  * cache[idx -12]              /* a(i-13) */
          - (seq_t)136 * cache[idx -13]              /* a(i-14) */
          + (seq_t)6   * cache[idx -14]              /* a(i-15) */
          + (seq_t)12  * cache[idx -15];             /* a(i-16) */
        cache[i - 1] = val;
    }

    cached_n = n;
    return cache[n - 1];
}
