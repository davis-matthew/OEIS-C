/*
 * A013982  Expansion of 1/(1-x^2-x^3-x^4-x^5).
 * https://oeis.org/A013982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-3)+a(n-4)+a(n-5) initial values: a(0)=1,
 *            a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=3
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 54, u64 n <= 106,
 *            u128 n <= 209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values */
static idx_t cache_size = 0;     /* number of entries in cache */

/* computes the nth term of A013982 */
seq_t a(idx_t n)
{
    /* initialise cache if first call */
    if (cache == NULL) {
        /* initial size at least 6 to hold base cases */
        cache_size = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) return 0;   /* allocation failed */

        /* explicit base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)3;
        /* remaining positions set to zero initially */
        for (idx_t i = 6; i < cache_size; ++i)
            cache[i] = (seq_t)0;
    }

    /* expand cache to cover requested n if necessary */
    if (n >= cache_size) {
        /* allocate larger array */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
        if (new_cache == NULL) return 0;   /* allocation failed */

        /* copy existing values */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* initialise new slots to zero (they will be computed below) */
        for (idx_t i = cache_size; i < new_size; ++i)
            new_cache[i] = (seq_t)0;

        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /* compute any newly needed values */
    for (idx_t i = 0; i <= n; ++i) {
        /* only compute if not already computed (i.e., when i >= 6 and cache[i] == 0 ) */
        if (i < 6) continue;              /* base values are already set */
        if (cache[i] != (seq_t)0) continue;

        seq_t sum = (seq_t)0;
        if (i >= 2) sum += cache[i - 2];
        if (i >= 3) sum += cache[i - 3];
        if (i >= 4) sum += cache[i - 4];
        if (i >= 5) sum += cache[i - 5];
        cache[i] = sum;
    }

    return cache[n];
}
