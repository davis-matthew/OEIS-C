/*
 * A177200  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=9, k=-1 and l=1.
 * https://oeis.org/A177200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Recurrence: a(n+1) = Σ_{p=0..n} (a(p)*a(n-p) + k) + l with k =
 *            -1, l = 1, a(0) = 1, a(1) = 9 -> For n ≥ 2: a(n) = Σ_{p=0..n-1}
 *            a(p)*a(n-1-p) + n*k + l
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_last = 0;

    /* first call – initialise cache with a(0) and a(1) */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache_last = 1;
    }

    /* grow cache if necessary (power‑of‑two growth) */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* reallocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute all missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i < 2) {          /* safely copy the initial values */
            cache[i] = (i == 0) ? (seq_t)1 : (seq_t)9;
            continue;
        }

        /* compute convolution sum Σ_{p=0}^{i-1} a(p)*a(i-1-p) */
        seq_t sum = 0;
        idx_t limit = i - 1;
        for (idx_t p = 0; p <= limit / 2; ++p) {
            seq_t prod = cache[p] * cache[limit - p];
            if (p * 2 != limit)          /* pair counted twice */
                sum += prod + prod;
            else
                sum += prod;
        }

        /* add (i)*k with k = -1, then add l = 1 */
        sum -= (seq_t)i;                 /* i*k, k = -1 */
        sum += (seq_t)1;                 /* l = 1 */

        cache[i] = sum;
    }
    cache_last = n;

    return cache[n];
}
