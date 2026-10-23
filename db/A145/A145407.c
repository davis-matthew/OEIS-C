/*
 * A145407  Number of Hamiltonian paths in O_6 X P_n.
 * https://oeis.org/A145407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 350*a(n-1) -22608*a(n-2) -17280*a(n-3) + 843264*a(n-4)
 *            a(1)=120, a(2)=41280, a(3)=6641952, a(4)=886927344,
 *            a(5)=105209243232
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_filled = 0;
    /* make sure the cache has room for indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    if (n > cache_filled) {
        for (idx_t i = cache_filled + 1; i <= n; ++i) {
            if      (i == 1) cache[i] = (seq_t)120;
            else if (i == 2) cache[i] = (seq_t)41280;
            else if (i == 3) cache[i] = (seq_t)6641952;
            else if (i == 4) cache[i] = (seq_t)886927344;
            else if (i == 5) cache[i] = (seq_t)105209243232;
            else {
                cache[i] = (seq_t)350 * cache[i-1]
                           - (seq_t)22608 * cache[i-2]
                           - (seq_t)17280 * cache[i-3]
                           + (seq_t)843264 * cache[i-4];
            }
        }
        cache_filled = n;
    }
    return cache[n];
}
