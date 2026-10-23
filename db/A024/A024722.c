/*
 * A024722  a(n) = Sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1), with
 *          a(1)=a(2)=1.
 * https://oeis.org/A024722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1), with
 *            a(1)=a(2)=1
 * Range:     u8 n <= 17, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* memoised implementation of OEIS sequence A024722 */
seq_t a(idx_t n)
{
    if (n < 1)                     /* below offset – undefined */
        return 0;

    /* Ensure the cache has room for indices 0..n (index 0 unused) */
    size_t need = (size_t)n + 1;   /* indices 0..n inclusive */
    if (need < 3) need = 3;        /* also hold for a(1) and a(2) */
    static seq_t *cache = NULL;    /* array 0..cache_sz-1 */
    static size_t cache_sz = 0;
    static idx_t cache_max = 0;    /* largest index already computed */

    if (need > cache_sz) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return 0;       /* allocation failed */
        cache = tmp;
        cache_sz = need;
    }

    /* initialise the known base cases once */
    if (cache_max < 1) {
        cache[1] = (seq_t)1;
        cache_max = 1;
    }
    if (cache_max < 2) {
        cache[2] = (seq_t)1;
        cache_max = 2;
    }

    /* compute any missing terms up to n */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        idx_t m = (k + 1) / 4;          /* floor((k+1)/4) */
        seq_t sum = 0;
        for (idx_t i = 1; i <= m; ++i) {
            seq_t left  = cache[2 * i - 1];
            seq_t right = cache[k - 2 * i + 1];
            sum += left * right;
        }
        cache[k] = sum;
    }

    /* keep cache_max as the maximum computed; do not shrink it */
    if (n > cache_max)
        cache_max = n;

    return cache[n];
}
