/*
 * A014001  Pisot sequence E(7,15), a(n)=[ a(n-1)^2/a(n-2)+1/2 ].
 * https://oeis.org/A014001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [ a(n-1)^2 / a(n-2) + 1/2 ], a(0) = 7, a(1) = 15
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 26, u64 n <= 56, u128 n <= 58,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static int init = 0;               /* 0 = not initialized, 1 = initialized */

    /* ensure the cache can hold indices up to n */
    ensure_cache(n, &cache, &cap);

    /* initialize the first two terms once */
    if (!init) {
        cache[0] = 7;
        cache[1] = 15;
        init = 1;
    }

    /* trivial cases */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* compute terms up to n if needed */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue;  /* already computed */

        seq_t a_prev     = cache[i-1];
        seq_t a_prevprev = cache[i-2];

        seq_t val;
        if (sizeof(seq_t) <= 8) {    /* uint8_t, uint16_t, uint32_t, uint64_t */
            unsigned __int128 prod = (unsigned __int128)a_prev * (unsigned __int128)a_prev;
            unsigned __int128 half = (unsigned __int128)a_prevprev >> 1;          /* floor(b/2) */
            unsigned __int128 num  = prod + half;                               /* a^2 + b/2 */
            val = (seq_t)(num / (unsigned __int128)a_prevprev);                  /* round to nearest */
        } else {    /* seq_t is 128-bit or arbitrary-precision */
            seq_t prod = a_prev * a_prev;     /* a^2 */
            seq_t half = a_prevprev >> 1;     /* floor(b/2) */
            seq_t num  = prod + half;         /* a^2 + b/2 */
            val = num / a_prevprev;           /* round to nearest */
        }
        cache[i] = val;
    }

    return cache[n];
}
