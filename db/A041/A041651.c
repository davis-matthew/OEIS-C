/*
 * A041651  Denominators of continued fraction convergents to sqrt(344).
 * https://oeis.org/A041651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 20810*a(n-10) - a(n-20)   for n > 19
 * with initial values:
 *   a(0)=1, a(1)=1, a(2)=2, a(3)=9, a(4)=11, a(5)=42,
 *   a(6)=53, a(7)=254, a(8)=307, a(9)=561,
 *   a(10)=20503, a(11)=21064, a(12)=41567, a(13)=187332,
 *   a(14)=228899, a(15)=874029, a(16)=1102928,
 *   a(17)=5285741, a(18)=6388669, a(19)=11674410.
 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = 0;
    static size_t cache_cap = 0;           /* number of elements allocated */

    /* initial 20 values */
    static const seq_t init[20] = {
        1, 1, 2, 9, 11, 42, 53, 254, 307, 561,
        20503, 21064, 41567, 187332, 228899,
        874029, 1102928, 5285741, 6388669, 11674410
    };

    /* if we don't have enough space, grow the cache */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == 0) {
            /* allocation failed – return 0 as sentinel value */
            return (seq_t)0;
        }
        cache = new_cache;

        /* compute new entries from the current cache size up to n */
        for (size_t i = cache_cap; i <= (size_t)n; ++i) {
            if (i < 20) {
                cache[i] = init[(idx_t)i];
            } else {
                cache[i] = (seq_t)20810 * cache[i - 10] - cache[i - 20];
            }
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
