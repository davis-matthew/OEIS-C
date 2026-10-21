/*
 * A214126  a(2n)=a(n-1)+a(n) and a(2n+1)=a(n+1) for n>=1, with a(0)=a(1)=1.
 * https://oeis.org/A214126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=1; a(2*n)=a(n-1)+a(n); a(2*n+1)=a(n+1)
 * Range:     u8 n <= 3063, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache   = NULL;
    static unsigned char *computed = NULL;
    static size_t capacity = 0;

    /* grow cache if needed */
    if (n >= capacity) {
        size_t newcap = capacity ? capacity * 2 : 1;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp_cache   = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        unsigned char *tmp_computed = (unsigned char *)realloc(computed, newcap * sizeof(unsigned char));
        if (!tmp_cache || !tmp_computed) exit(EXIT_FAILURE);
        for (size_t i = capacity; i < newcap; ++i) tmp_computed[i] = 0;
        cache     = tmp_cache;
        computed  = tmp_computed;
        capacity  = newcap;
    }

    /* return cached value if already computed */
    if (computed[n]) return cache[n];

    seq_t result;
    if (n == 0 || n == 1) {
        result = 1;
    } else if ((n & 1) == 0) {            /* even: n = 2*m */
        idx_t m = n >> 1;
        result = a(m - 1) + a(m);
    } else {                              /* odd: n = 2*m+1, m>=1 */
        idx_t m = (n - 1) >> 1;
        result = a(m + 1);
    }

    cache[n]     = result;
    computed[n]  = 1;
    return result;
}
