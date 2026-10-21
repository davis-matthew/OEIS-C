/*
 * A051164  Sequence is defined by property that (a0,a1,a2,a3,...) = binomial
 *          transform of (a0,a0,a0,a1,a1,a1,a2,a2,a2,a3,a3,a3,...).
 * https://oeis.org/A051164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, eigen
 * Formula:   * a(n) = Σ_{k=0..n} binom(n,k) * a(floor(k/3)), * a(0) = 1. *
 *            This direct binomial‑transform computation is memoised; any
 *            order of * calls returns the correct value. The implementation
 *            is O(n²) * (≈ ½ n(n+1) elementary operations for the largest
 *            requested n), * which is well within the 2 s limit...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed a(n) values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of stored values (≥ cache_len) */

    /* initialise cache with a(0) = 1 */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) abort();
        cache[0] = (seq_t)1;          /* a(0) = 1 */
    }

    /* extend cache up to requested n */
    if (n >= cache_len) {
        /* Reallocate to hold indices 0 … n */
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) abort();
        cache = tmp;

        /* compute all new terms */
        for (idx_t cur = cache_len; cur <= n; ++cur) {
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;          /* binom(cur,0) */

            /* iterate over k = 0 … cur */
            for (idx_t k = 0; k <= cur; ++k) {
                seq_t term = binom * cache[k / 3];
                sum += term;

                /* update binom to binom(cur, k+1) */
                if (k < cur) {
                    seq_t numer = (seq_t)(cur - k);
                    seq_t denom = (seq_t)(k + 1);
                    binom = (binom * numer) / denom;
                }
            }
            cache[cur] = sum;
        }
        cache_len = new_len;
    }

    return cache[n];
}
