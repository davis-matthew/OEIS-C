/*
 * A192735  Left edge of the triangle in A033291.
 * https://oeis.org/A192735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 4*a(n-4) + 2*a(n-5) -
 *            a(n-6) + 2*a(n-7) - a(n-8) (n ≥ 9) Initial values: a(1)=1,
 *            a(2)=2, a(3)=6, a(4)=16, a(5)=30, a(6)=54, a(7)=91, a(8)=136.
 *            These values are equivalent to a(n) = A087483(n‑2) * n
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation – indices 0..max computed, 0 unused */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static idx_t  cache_max  = 0;

    /* ensure we have space for index n (including index 0) */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();  /* memory exhausted – unlikely for n ≤ 10000 */
        cache      = tmp;
        cache_size = new_size;
    }

    /* initialise the first 8 terms once */
    if (cache_max == 0) {
        /* we need indices up to 8 */
        if (cache_size < 9) {
            seq_t *tmp = (seq_t *)realloc(cache, 9 * sizeof(seq_t));
            if (!tmp) abort();
            cache      = tmp;
            cache_size = 9;
        }
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)16;
        cache[5] = (seq_t)30;
        cache[6] = (seq_t)54;
        cache[7] = (seq_t)91;
        cache[8] = (seq_t)136;
        cache_max = 8;
    }

    if (n <= cache_max) {
        return cache[n];
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;

        /* positive part: 2*a(i‑1) + 2*a(i‑3) + 2*a(i‑5) + 2*a(i‑7) */
        pos += cache[i - 1] * (seq_t)2;
        pos += cache[i - 3] * (seq_t)2;
        pos += cache[i - 5] * (seq_t)2;
        pos += cache[i - 7] * (seq_t)2;

        /* negative part:   a(i‑2) + 4*a(i‑4) + a(i‑6) + a(i‑8) */
        neg += cache[i - 2];
        neg += cache[i - 4] * (seq_t)4;
        neg += cache[i - 6];
        neg += cache[i - 8];

        cache[i] = pos - neg;   /* pos ≥ neg for all n ≥ 9 */
    }

    cache_max = n;
    return cache[n];
}
