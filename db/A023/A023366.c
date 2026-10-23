/*
 * A023366  a(n+1) = a(n) converted to base 9 from base 2 (written in base
 *          10).
 * https://oeis.org/A023366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 2, a(n+1) = a(n) interpreted as a base‑9 number by
 *            treating the binary digits of a(n) as base‑9 digits
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[ i ] = a(i) for i >= 1 */
    static idx_t cacheSize = 0;      /* largest i for which cache[i] is valid */

    if (n == 0)
        return 0;

    /* initialise cache for the first call */
    if (cache == NULL) {
        cacheSize = 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * (cacheSize + 1));
        cache[1] = (seq_t)2;   /* a(1) = 2 */
    }

    /* grow cache if necessary */
    if (n > cacheSize) {
        cache = (seq_t*)realloc(cache,
                                sizeof(seq_t) * (n + 1));
        for (idx_t i = cacheSize + 1; i <= n; ++i) {
            seq_t x = cache[i - 1];   /* a(i-1) */
            seq_t y = 0;              /* a(i) to compute */
            seq_t power = 1;
            while (x > 0) {
                if (x & (seq_t)1)
                    y += power;
                power *= (seq_t)9;
                x >>= 1;
            }
            cache[i] = y;
        }
        cacheSize = n;
    }

    return cache[n];
}
