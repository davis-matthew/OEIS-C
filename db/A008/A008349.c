/*
 * A008349  Crystal ball sequence for E_8 lattice.
 * https://oeis.org/A008349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) initial
 *            values: a(0)=1, a(1)=241, a(2)=9361, a(3)=131041, a(4)=996001,
 *            a(5)=5109841, a(6)=20015281, a(7)=64495681, a(8)=179375041
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 12, u64 n <= 196, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* dynamic cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* initialise cache with first nine terms */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;        /* allocation failed */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)241;
        cache[2] = (seq_t)9361;
        cache[3] = (seq_t)131041;
        cache[4] = (seq_t)996001;
        cache[5] = (seq_t)5109841;
        cache[6] = (seq_t)20015281;
        cache[7] = (seq_t)64495681;
        cache[8] = (seq_t)179375041;
        cache_len = 9;
    }

    /* if the requested term is already computed */
    if (n < cache_len)
        return cache[n];

    /* compute terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* grow the cache if needed */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;   /* allocation failed */
            cache = tmp;
        }

        /* recurrence: a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3)
                          - 126*a(n-4) + 126*a(n-5) - 84*a(n-6)
                          + 36*a(n-7) - 9*a(n-8) + a(n-9) */
        seq_t pos = (seq_t)9  * cache[i-1]    /* +9  * a(n-1)   */
                  + (seq_t)84 * cache[i-3]    /* +84 * a(n-3)   */
                  + (seq_t)126 * cache[i-5]    /* +126* a(n-5)   */
                  + (seq_t)36 * cache[i-7]    /* +36 * a(n-7)   */
                  + cache[i-9];               /* +1  * a(n-9)   */

        seq_t neg = (seq_t)36 * cache[i-2]    /* -36 * a(n-2)   */
                  + (seq_t)126 * cache[i-4]    /* -126* a(n-4)   */
                  + (seq_t)84 * cache[i-6]    /* -84 * a(n-6)   */
                  + (seq_t)9  * cache[i-8];   /* -9  * a(n-8)   */

        cache[i] = pos - neg;
        ++cache_len;
    }

    return cache[n];
}
