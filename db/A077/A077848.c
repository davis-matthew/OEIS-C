/*
 * A077848  Expansion of (1-x)^(-1)/(1-2*x-x^2-2*x^3).
 * https://oeis.org/A077848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4); initial values:
 *            a(0)=1, a(1)=3, a(2)=8, a(3)=22;
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[4] = { (seq_t)1, (seq_t)3, (seq_t)8, (seq_t)22 };

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n < 4) return base_vals[n];

    /* cache previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* ensure cache is large enough to hold the first four values */
    if (cache_len < 4) {
        idx_t new_len = 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache[0] = base_vals[0];
        cache[1] = base_vals[1];
        cache[2] = base_vals[2];
        cache[3] = base_vals[3];
        cache_len = new_len;
    }

    /* extend the cache up to the requested index if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t ai1 = cache[i-1];
            seq_t ai2 = cache[i-2];
            seq_t ai3 = cache[i-3];
            seq_t ai4 = cache[i-4];
            seq_t val = ai1 * (seq_t)3;
            val += ai3;
            val -= ai2;
            val -= ai4 * (seq_t)2;
            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[n];
}
