/*
 * A123351  Kekulé numbers for certain benzenoids (see the Cyvin-Gutman book
 *          for details).
 * https://oeis.org/A123351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) -21*a(n-2) +35*a(n-3) -35*a(n-4) +21*a(n-5)
 *            -7*a(n-6) + a(n-7) for n ≥ 8 Initial values: a(0)=1, a(1)=4,
 *            a(2)=30, a(3)=146, a(4)=517, a(5)=1476, a(6)=3614, a(7)=7890
 *            (Sequence A123351 – Kekulé numbers for certain benzenoids)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static const seq_t init[8] = {1, 4, 30, 146, 517, 1476, 3614, 7890};
    static seq_t *cache = NULL;          /* array of computed terms */
    static idx_t cache_len = 0;          /* highest index stored in cache */

    /* Initialise cache with the first 8 terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(8 * sizeof(seq_t));
        if (cache == NULL)
            return 0;  /* allocation failure – fallback */
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        cache_len = 7;
    }

    /* Return early if the requested term is already in cache */
    if (n <= cache_len)
        return cache[n];

    /* Grow the cache to accommodate index n */
    size_t new_len = (size_t)(n + 1);
    if (new_len > (size_t)cache_len + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)
            return 0;      /* allocation failure – fallback */
        cache = tmp;
    }

    /* Compute terms up to n (inclusive) */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i < 8)                /* indices < 8 are already initialised */
            continue;

        /* Positives: 7*a(i-1) + 35*a(i-3) + 21*a(i-5) + a(i-7) */
        seq_t pos = 7 * cache[i - 1];
        pos += 35 * cache[i - 3];
        pos += 21 * cache[i - 5];
        pos += cache[i - 7];

        /* Negatives: 21*a(i-2) + 35*a(i-4) + 7*a(i-6) */
        seq_t neg = 21 * cache[i - 2];
        neg += 35 * cache[i - 4];
        neg += 7 * cache[i - 6];

        cache[i] = pos - neg;       /* pos ≥ neg for all computed terms */
    }

    cache_len = n;
    return cache[n];
}
