/*
 * A029399  Expansion of 1/((1-x^5)*(1-x^7)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A029399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5)+a(n-7)+a(n-9)-a(n-14)-a(n-16)-a(n-17)-a(n-19)
 *            +a(n-24)+a(n-26)+a(n-28)-a(n-33) Initial terms: a(0)=1, a(n)=0
 *            for 1≤n<5. The function memoises computed values for arbitrary
 *            access order
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with a(0) = 1 */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) abort();
        cache[0] = (seq_t)1;
    }

    if (n >= cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) abort();
        cache = tmp;

        /* initialise new slots to 0 */
        for (idx_t i = old_len; i < new_len; ++i)
            cache[i] = (seq_t)0;

        /* compute all missing values */
        for (idx_t i = old_len; i <= n; ++i) {
            seq_t val = (seq_t)0;
            /* add all positive contributions first */
            if (i >= 5)  val += cache[i - 5];
            if (i >= 7)  val += cache[i - 7];
            if (i >= 9)  val += cache[i - 9];
            if (i >= 24) val += cache[i - 24];
            if (i >= 26) val += cache[i - 26];
            if (i >= 28) val += cache[i - 28];
            /* subtract all negative contributions */
            if (i >= 14) val -= cache[i - 14];
            if (i >= 16) val -= cache[i - 16];
            if (i >= 17) val -= cache[i - 17];
            if (i >= 19) val -= cache[i - 19];
            if (i >= 33) val -= cache[i - 33];
            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[n];
}
