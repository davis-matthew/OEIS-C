/*
 * A145132  Expansion of x/((1 - x - x^4)*(1 - x)^3).
 * https://oeis.org/A145132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - 3*a(n-5) + 3*a(n-6) -
 *            a(n-7) initial values: a(0)=0, a(1)=1, a(2)=4, a(3)=10, a(4)=20,
 *            a(5)=36, a(6)=61, a(7)=99
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 59, u64 n <= 128, u128 n <= 266,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_eff = 0;
    static const seq_t base[8] = {0, 1, 4, 10, 20, 36, 61, 99};

    /* Grow cache if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) return (seq_t)0; /* Allocation failed */
        cache = newp;
        cache_cap = new_cap;
    }

    /* Compute missing values on demand */
    if (cache_eff <= n) {
        for (idx_t i = cache_eff; i <= n; ++i) {
            if (i < 8) {
                cache[i] = base[i];
            } else {
                cache[i] = 4 * cache[i - 1]
                           - 6 * cache[i - 2]
                           + 4 * cache[i - 3]
                           - 3 * cache[i - 5]
                           + 3 * cache[i - 6]
                           - cache[i - 7];
            }
        }
        cache_eff = n + 1;
    }
    return cache[n];
}
