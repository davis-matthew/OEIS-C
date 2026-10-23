/*
 * A167638  Number of Dyck paths of semilength n, having no ascents and no
 *          descents of length 1, and having no peaks at even level.
 * https://oeis.org/A167638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n+1)*a(n) + a(n-1) + (-4*n+7)*a(n-2) + 2*(-n+2)*a(n-3) + a(n-4)
 *            - a(n-5) + (n-7)*a(n-6) = 0 Initial values: a(0)=1, a(1)=0,
 *            a(2)=0, a(3)=1, a(4)=0, a(5)=2
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 31, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with first six terms */
    if (cache_len == 0)
    {
        cache_len = 6;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return 0; /* allocation failure – unlikely in tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)0;
        cache[5] = (seq_t)2;
    }
    if (n < cache_len)
        return cache[n];

    /* Grow cache if needed */
    if (n + 1 > cache_len)
    {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure – unlikely */
        cache = tmp;
        cache_len = n + 1;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = 6; i <= n; ++i)
    {
        /* Compute numerator: positives minus negatives */
        seq_t pos = 0;
        seq_t neg = 0;

        pos += (seq_t)(4*i - 7) * cache[i - 2];
        pos += (seq_t)(2*i - 4) * cache[i - 3];
        pos += cache[i - 5];

        if (i <= 6)
            pos += (seq_t)(7 - i) * cache[i - 6];
        else
            neg += (seq_t)(i - 7) * cache[i - 6];

        neg += cache[i - 1];
        neg += cache[i - 4];

        /* Sequence satisfies (i+1)*a(i) = pos - neg */
        cache[i] = (pos - neg) / (i + 1);
    }

    return cache[n];
}
