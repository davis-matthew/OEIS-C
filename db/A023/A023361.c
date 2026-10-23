/*
 * A023361  Number of compositions of n into positive triangular numbers.
 * https://oeis.org/A023361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(0) = 1; for n ≥ 1, a(n) = Σ_{t = k(k+1)/2 ≤ n} a(n – t)
 *            (sum over all triangular numbers ≤ n)
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 52, u64 n <= 102,
 *            u128 n <= 204, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] stores a(i) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* allocated array size */
    static idx_t  cached_max = (idx_t)-1;  /* largest n that has been computed */

    /* enlarge cache if needed (use power‑of‑two growth) */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;          /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first element if cache is empty */
    if (cached_max == (idx_t)-1) {
        cache[0] = 1;                /* a(0) = 1 */
        cached_max = 0;
    }

    /* compute all values up to n that are not yet cached */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1;; ++k) {
            idx_t t = k * (k + 1) / 2;   /* k(k+1)/2 */
            if (t > i) break;
            sum += cache[i - t];
        }
        cache[i] = sum;
    }
    cached_max = n;

    return cache[n];
}
