/*
 * A041454  Numerators of continued fraction convergents to sqrt(243).
 * https://oeis.org/A041454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent to √243. Recurrence:
 *            p_{-2}=0, p_{-1}=1, p_n = a_n * p_{n-1} + p_{n-2} a_0 = 15; for
 *            n≥1, a_n = period[(n-1) mod 10] where period =
 *            {1,1,2,3,15,3,2,1,1,30}
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of already computed numerators */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_last = (idx_t)-1;   /* last index computed */

    /* Periodic part of the continued fraction after the initial 15 */
    static const idx_t period[10] = {1, 1, 2, 3, 15, 3, 2, 1, 1, 30};
    const idx_t period_len = 10;

    /* Ensure the cache is large enough */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t k = cache_last + 1; k <= n; ++k) {
        seq_t p_k;
        seq_t a_k;

        if (k == 0) {
            a_k = (seq_t)15;                     /* a_0 */
            /* p_0 = a_0 * p_{-1} + p_{-2} = 15*1 + 0 */
            p_k = a_k * (seq_t)1 + (seq_t)0;
        } else if (k == 1) {
            a_k = (seq_t)period[0];               /* a_1 */
            /* p_1 = a_1 * p_0 + p_{-1} = 1*15 + 1 */
            p_k = a_k * cache[0] + (seq_t)1;
        } else {
            a_k = (seq_t)period[(k - 1) % period_len];
            p_k = a_k * cache[(size_t)(k - 1)] + cache[(size_t)(k - 2)];
        }
        cache[(size_t)k] = p_k;
    }
    cache_last = n;

    return cache[(size_t)n];
}
