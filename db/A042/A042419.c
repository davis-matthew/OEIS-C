/*
 * A042419  Denominators of continued fraction convergents to sqrt(737).
 * https://oeis.org/A042419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=6, a(n)= a(n-1)*period[(n-1)%14]+a(n-2) for n>=2
 *            where period = [6,1,3,3,7,2,4,2,7,3,3,1,6,54] is the partial
 *            quotients of the simple continued fraction of sqrt(737)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Partial quotients after the initial term for sqrt(737) */
static const seq_t period[14] = {
    (seq_t)6, (seq_t)1, (seq_t)3, (seq_t)3,
    (seq_t)7, (seq_t)2, (seq_t)4, (seq_t)2,
    (seq_t)7, (seq_t)3, (seq_t)3, (seq_t)1,
    (seq_t)6, (seq_t)54
};

seq_t a(idx_t n) {
    /* Cached denominators */
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t cache_len = 0;  /* number of terms actually stored */
    static size_t cache_cap = 0;  /* allocated size of cache */

    /* Ensure we can index using size_t */
    size_t idx = (size_t)n;

    /* Initial allocation */
    if (!cache) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure – return 0 */
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cache_len = 1;
    }

    /* Grow the cache if we need more space */
    if (idx + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < idx + 1) new_cap = new_cap * 2 + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to idx (inclusive) */
    if (cache_len <= idx) {
        size_t start = cache_len;          /* first index to compute */
        for (size_t i = start; i <= idx; ++i) {
            /* period index: partial quotient a[i] = period[(i-1)%14] */
            seq_t p = period[(i - 1) % 14];
            seq_t cur;
            if (i >= 2)
                cur = p * cache[i - 1] + cache[i - 2];
            else
                /* i == 1: use cache[0] and 0 for cache[-1] */
                cur = p * cache[0] + (seq_t)0;
            cache[i] = cur;
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
