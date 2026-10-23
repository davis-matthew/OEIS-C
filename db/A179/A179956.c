/*
 * A179956  a(n) is the smallest integer that contains a(n-1) but does not
 *          begin with a(n-1).
 * https://oeis.org/A179956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = 10^(n-1) + a(n-1) for n>2, a(1)=1, a(2)=21
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   grow_seq, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    if (n == 0) {
        return 0;   /* undefined offset, return 0 */
    }

    if (cache_len < n) {
        grow_seq(&cache, &cache_cap, (size_t)n);

        if (cache_len < 1) {
            cache[0] = 1;
            cache_len = 1;
        }
        if (cache_len < 2) {
            cache[1] = 21;
            cache_len = 2;
        }

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t p = pow10((idx_t)(i - 1));
            cache[i - 1] = p + cache[i - 2];
        }
        cache_len = n;
    }

    return cache[n - 1];
}
