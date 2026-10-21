/*
 * A053298  Partial sums of A027964.
 * https://oeis.org/A053298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 8 a(n) = a(n-1) + a(n-2) + (3*n+4)*C(n+3,3)/4
 *            for n >= 2 where C(k,3) = k*(k-1)*(k-2)/6 (This computes
 *            A053298, the partial sums of A027964.)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 35, u64 n <= 81, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of computed terms */
    static idx_t   cache_len = 0;    /* number of valid entries in cache */

    /* lazy initialization of the cache with first two terms */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (cache == NULL) return 0;    /* out‑of‑memory → undefined behavior */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
    }

    /* Extend cache up to n (inclusive) if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (tmp == NULL) return 0;      /* out‑of‑memory → undefined behavior */
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            /* compute the binomial C(i+3,3) = (i+3)(i+2)(i+1)/6 */
            seq_t c = (seq_t)(i + 3) * (i + 2) * (i + 1) / 6;
            /* compute the incremental term (3*i+4)*c/4 */
            seq_t incr = (seq_t)(3 * i + 4) * c / 4;
            /* recurrence: a(i) = a(i-1) + a(i-2) + incr */
            cache[i] = cache[i - 1] + cache[i - 2] + incr;
        }
        cache_len = new_len;
    }

    return cache[n];
}
