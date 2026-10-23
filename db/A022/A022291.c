/*
 * A022291  Expansion of 1/((1-x)*(1-5*x)*(1-6*x)*(1-9*x)).
 * https://oeis.org/A022291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n‑1) -149*a(n‑2) +399*a(n‑3) -270*a(n‑4) Initial
 *            terms: a(0)=1, a(1)=21, a(2)=292, a(3)=3402
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of already computed terms.  The array grows only when we
     * need a larger n, making repeated calls efficient. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values currently stored */

    /* Initialise the cache with the four known initial values. */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        /* Shade conversion from integer to seq_t is safe for all builds. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)21;
        cache[2] = (seq_t)292;
        cache[3] = (seq_t)3402;
    }

    /* If the requested index is already computed, return it. */
    if (n < cache_len) return cache[n];

    /* Grow the cache to accommodate n.  All new terms are computed
     * once using the recurrence and stored for future calls.  The
     * recurrence itself uses only the allowed operations (+ - *). */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t ai = cache[i-1] * (seq_t)21;
            ai -= cache[i-2] * (seq_t)149;
            ai += cache[i-3] * (seq_t)399;
            ai -= cache[i-4] * (seq_t)270;
            cache[i] = ai;
        }
        cache_len = new_len;
    }

    return cache[n];
}
