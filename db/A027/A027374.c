/*
 * A027374  Coordination sequence T2 for Zeolite Code SAT.
 * https://oeis.org/A027374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence derived from (1 - x)^3 (1 -
 *            x + x^2 - x^3 + x^4) (1 + x^4) (1 + x + x^2 + x^3 + x^4) (1 +
 *            x^3 + x^6) A(x) = (1 + x^3) (1 + x + x^2 + 2 x^3 + 4 x^4 + 5 x^5
 *            + 3 x^6 + 2 x^7 + 6 x^8 + 8 x^9 + 6 x^10 + 2 x^11 + 3 x^12 + 5
 *            x^13 + 4 x^14 + 2 x^15 + x^16 + x^17 + x^1...
 * Range:     u8 n <= 11, u16 n <= 184, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* 21‑term recurrence coefficients (denominator of the gf)   */
    static const int dcoeff[22] = {
        1,  -3,   4,  -3,   2,  -3,   5, -6,   5,  -4,   4,  -4,   4,
       -5,   6,  -5,   3,  -2,   3,  -4,   3,  -1
    };
    /*  numerator coefficients (for n ≤ 21)                       */
    static const int ncoeff[22] = {
        1, 1, 1, 3, 5, 6, 5, 6,11,11, 8, 8,11,11, 6, 5,6,5,3,1,1,1
    };

    static seq_t *cache = NULL;      /* memoised sequence values          */
    static size_t cache_len = 0;     /* number of values stored           */

    /* Allocate and initialise the first 22 terms (0 … 21) */
    if (cache == NULL) {
        const size_t init_len = 22;
        cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (!cache) return 0;          /* memory error – return 0 as a fallback */
        cache_len = init_len;

        for (idx_t i = 0; i < 22; ++i) {
            seq_t posSum = 0, negSum = 0;
            idx_t maxk = (i < 21) ? i : 21;   /* only available previous terms */
            for (idx_t k = 1; k <= maxk; ++k) {
                if (dcoeff[k] > 0)
                    posSum += (seq_t)dcoeff[k] * cache[i - k];
                else if (dcoeff[k] < 0)
                    negSum += (seq_t)(-dcoeff[k]) * cache[i - k];
            }
            if (i <= 21) {
                seq_t tmp = (seq_t)ncoeff[i] + negSum;
                cache[i] = tmp - posSum;
            } else {
                cache[i] = negSum - posSum;
            }
        }
    }

    /* Return cached value if already computed */
    if (n < cache_len) return cache[n];

    /* Extend cache up to requested index n */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;            /* memory error – return 0 as fallback  */
    cache = tmp;

    /* Compute missing terms sequentially */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t posSum = 0, negSum = 0;
        idx_t maxk = (i < 21) ? i : 21;
        for (idx_t k = 1; k <= maxk; ++k) {
            if (dcoeff[k] > 0)
                posSum += (seq_t)dcoeff[k] * cache[i - k];
            else if (dcoeff[k] < 0)
                negSum += (seq_t)(-dcoeff[k]) * cache[i - k];
        }
        if (i <= 21) {
            seq_t tmpval = (seq_t)ncoeff[i] + negSum;
            cache[i] = tmpval - posSum;
        } else {
            cache[i] = negSum - posSum;
        }
    }
    cache_len = new_len;
    return cache[n];
}
