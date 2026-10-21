/*
 * A090409  a(n) = (7*8^n + 2*(-1)^n)/9.
 * https://oeis.org/A090409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (7*8^n + 2*(-1)^n)/9 Recurrence: a(0)=1, a(1)=6,
 *            a(n)=7*a(n-1)+8*a(n-2) for n>1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache previous values so we can answer any call order efficiently */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialise cache if first call or to ensure space for n */
    if (!cache) {
        cache_cap = (n > 1 ? n + 1 : 2);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0; /* allocation failure – unlikely */
        cache[0] = (seq_t)1;
        if (cache_cap > 1) cache[1] = (seq_t)6;
        cache_len = 2;
    }

    /* grow cache if needed */
    while (cache_len <= n) {
        if (cache_len == cache_cap) {
            /* increase capacity */
            cache_cap = (cache_cap << 1) + 1;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            if (!cache) return (seq_t)0; /* allocation failure – unlikely */
        }
        /* recurrence: a(i) = 7*a(i-1) + 8*a(i-2) */
        seq_t val = cache[cache_len - 1] * (seq_t)7 +
                    cache[cache_len - 2] * (seq_t)8;
        cache[cache_len] = val;
        cache_len++;
    }

    return cache[n];
}
