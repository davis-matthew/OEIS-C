/*
 * A015469  q-Fibonacci numbers for q=11, scaling a(n-2).
 * https://oeis.org/A015469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 11^(n-2)*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
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
    static idx_t cache_cap = 0;
    static seq_t pow_current = 1;      /* 11^(next_index-2) */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* initialise cache on first use */
    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache_len = 2;
        pow_current = 1;          /* will be used for index 2 */
    }

    /* use cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* grow cache as needed (doubling strategy) */
    if ((idx_t)(n + 1) > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 2;
        while (new_cap < (idx_t)(n + 1))
            new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t a_ip1 = cache[i - 1];
        seq_t a_ip2 = cache[i - 2];
        seq_t a_i   = a_ip1 + pow_current * a_ip2;
        cache[i] = a_i;
        pow_current *= 11;         /* prepare 11^(i+1-2) for next i */
    }
    cache_len = n + 1;
    return cache[n];
}
