/*
 * A234560  Number of (n+1) X (4+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1)+30*a(n-2)-132*a(n-3)-331*a(n-4)+1720*a(n-5) *
 *            +1530*a(n-6)-11280*a(n-7)-1324*a(n-8)+39136*a(n-9) *
 *            -12360*a(n-10)-67968*a(n-11)+39456*a(n-12)+46080*a(n-13) *
 *            -34560*a(n-14) for n > 14 * Initial values: a(1)=1954,
 *            a(2)=3384, a(3)=5710, a(4)=11982, * a(5)=23830, a(6)=57210,
 *            a(7)=128...
 * Range:     u8 none, u16 n <= 6, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; undefined for n = 0 */

    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Ensure space for the first n terms (indices 0 .. n-1) */
    ensure_cache(n, &cache, &cap);

    /* Seed the first 14 terms (a(1) .. a(14)) */
    static const seq_t seed[14] = {
        (seq_t)1954, (seq_t)3384, (seq_t)5710, (seq_t)11982, (seq_t)23830,
        (seq_t)57210, (seq_t)128614, (seq_t)339234, (seq_t)831334,
        (seq_t)2348514, (seq_t)6096550, (seq_t)18108882, (seq_t)48774790,
        (seq_t)150230370
    };
    for (idx_t i = 0; i < 14 && i < n; ++i)
        if (!cache[i]) cache[i] = seed[i];

    /* Compute terms beyond the seed using the recurrence */
    for (idx_t i = 14; i < n; ++i) {
        if (cache[i]) continue;            /* already computed */
        seq_t t =  4 * cache[i-1];
        t += 30 * cache[i-2];
        t -= 132 * cache[i-3];
        t -= 331 * cache[i-4];
        t += 1720 * cache[i-5];
        t += 1530 * cache[i-6];
        t -= 11280 * cache[i-7];
        t -= 1324 * cache[i-8];
        t += 39136 * cache[i-9];
        t -= 12360 * cache[i-10];
        t -= 67968 * cache[i-11];
        t += 39456 * cache[i-12];
        t += 46080 * cache[i-13];
        t -= 34560 * cache[i-14];
        cache[i] = t;
    }

    return cache[n-1];
}
