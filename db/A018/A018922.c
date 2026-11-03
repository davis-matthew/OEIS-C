/*
 * A018922  Define the generalized Pisot sequence T(a(0),a(1)) by: a(n+2) is
 *          the greatest integer such that a(n+2)/a(n+1) < a(n+1)/a(n). This
 *          is T(8,16).
 * https://oeis.org/A018922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-5), a(0) = 8, a(1) = 16, a(2) = 31, a(3) =
 *            60, a(4) = 116
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 30, u64 n <= 64, u128 n <= 131,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 986 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;        /* number of terms stored in cache */

    idx_t old_len = cache_len;
    if (n >= old_len)
    {
        /* enlarge cache to hold terms up to n */
        idx_t new_len = n + 1;
        cache = mem_resize(seq_t, cache, (size_t)new_len);
        if (!cache) return 0;          /* allocation failed */

        /* initialise the first five terms if not yet done */
        if (old_len < 5)
        {
            if (0 < new_len) cache[0] = 8;
            if (1 < new_len) cache[1] = 16;
            if (2 < new_len) cache[2] = 31;
            if (3 < new_len) cache[3] = 60;
            if (4 < new_len) cache[4] = 116;
        }

        /* start generating from max(old_len,5) up to n */
        idx_t start = old_len;
        if (start < 5) start = 5;
        for (idx_t i = start; i < new_len; ++i)
        {
            /* a[i] = 2*a[i-1] - a[i-5] */
            cache[i] = mul_small(cache[i-1], 2) - cache[i-5];
        }
        cache_len = new_len;
    }
    return cache[n];
}
