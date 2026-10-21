/*
 * A052587  Expansion of e.g.f. x^2*(1-x)/(1-2*x).
 * https://oeis.org/A052587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n = 0,1; a(2) = 2; a(3) = 6; Recurrence: a(n) =
 *            2*n*a(n-1) for all n >= 4
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cache[0..cache_len-1] holds computed terms */
    static idx_t cache_len = 0;    /* number of terms stored */

    /* initialise cache with first four terms */
    if (cache_len == 0) {
        cache_len = 4;                    /* indices 0..3 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;             /* fall back on zero if out of memory */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
    }

    /* expand cache if needed */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;               /* fallback on zero on realloc failure */
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            /* a(i) = 2*i*a(i-1) for i >= 4 */
            cache[i] = cache[i-1] * ((seq_t)(2*i));
        }
        cache_len = new_len;
    }

    return cache[n];
}
