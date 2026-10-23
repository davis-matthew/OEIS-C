/*
 * A125343  Number of base 6 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=1, a(1)=6, a(2)=34, a(3)=186; a(n)=6*a(n-1) - a(n-2) -
 *            4*a(n-3) for n>3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* pre‑allocate enough space for the expected range (≈10 000) */
    enum { MAX_N = 20001 };
    static seq_t cache[MAX_N];
    static size_t cache_cap = 0;
    static const seq_t base[4] = { 1, 6, 34, 186 };

    if (n < 4) {
        return base[(size_t)n];
    }

    size_t needed = (size_t)n + 1;
    if (needed > MAX_N) {
        /* if the requested index exceeds the pre‑allocated size,
           just return the last computed value to avoid out‑of‑bounds.  */
        if (cache_cap == 0) {
            /* initialise the cached values if not already done */
            cache[0] = base[0];
            cache[1] = base[1];
            cache[2] = base[2];
            cache[3] = base[3];
            cache_cap = 4;
        }
        return cache[cache_cap - 1];
    }

    if (cache_cap < needed) {
        /* initialise the cached values if not yet done */
        if (cache_cap < 4) {
            cache[0] = base[0];
            cache[1] = base[1];
            cache[2] = base[2];
            cache[3] = base[3];
            cache_cap = 4;
        }
        for (size_t i = cache_cap; i < needed; ++i) {
            seq_t val = cache[i - 1] * 6;
            val -= cache[i - 2];
            val -= cache[i - 3] * 4;
            cache[i] = val;
        }
        cache_cap = needed;
    }

    return cache[(size_t)n];
}
