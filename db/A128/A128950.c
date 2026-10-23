/*
 * A128950  a(n) is equal to the number of positive integers m less than or
 *          equal to 10^n such that m is not divisible by the prime 7 and is
 *          not divisible by at least one of the primes 2, 3 and 5.
 * https://oeis.org/A128950
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^n - floor(10^n/7) - floor(10^n/30) + floor(10^n/210)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cached powers of 10: cache[i] = 10^i  (i >= 0) */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap? cache_cap << 1 : 1;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* ensure the cache contains 10^n */
    if (n >= cache_len) {
        grow_cache(n + 1);
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = cache[i-1] * (seq_t)10;
        }
        cache_len = n + 1;
    }

    seq_t t  = cache[n];
    seq_t q7  = t / (seq_t)7;
    seq_t q30 = t / (seq_t)30;
    seq_t q210 = t / (seq_t)210;
    return t - q7 - q30 + q210;
}
