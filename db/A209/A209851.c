/*
 * A209851  1/4 the number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock
 *          having one or four distinct clockwise edge differences.
 * https://oeis.org/A209851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 29*a(n-2) - 28*a(n-3) - 110*a(n-4) + 82*a(n-5)
 *            - 8*a(n-6) (n ≥ 7) initial terms: a(1)=25, a(2)=182, a(3)=1308,
 *            a(4)=9455, a(5)=68201, a(6)=492373
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n < 1, but return 0 */

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialize the cache with the first six values */
    if (!cache) {
        cache_len = 6;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return 0;  /* allocation failure: give up */
        cache[0] = (seq_t)25;
        cache[1] = (seq_t)182;
        cache[2] = (seq_t)1308;
        cache[3] = (seq_t)9455;
        cache[4] = (seq_t)68201;
        cache[5] = (seq_t)492373;
    }

    /* Extend the cache if the requested n is larger */
    if (n > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!new_cache) return 0;  /* allocation failure: give up */
        cache = new_cache;
        /* Compute missing terms using the recurrence */
        for (idx_t i = cache_len; i < n; ++i) {
            seq_t a1 = cache[i - 1];   /* a(n-1) */
            seq_t a2 = cache[i - 2];   /* a(n-2) */
            seq_t a3 = cache[i - 3];   /* a(n-3) */
            seq_t a4 = cache[i - 4];   /* a(n-4) */
            seq_t a5 = cache[i - 5];   /* a(n-5) */
            seq_t a6 = cache[i - 6];   /* a(n-6) */

            seq_t pos = 4 * a1;
            pos += 29 * a2;
            pos += 82 * a5;

            seq_t neg = 28 * a3;
            neg += 110 * a4;
            neg += 8 * a6;

            cache[i] = pos - neg;      /* pos >= neg for all n ≥ 7 */
        }
        cache_len = n;
    }

    return cache[n - 1];   /* cache is 0‑based, a(1) is cache[0] */
}
