/*
 * A097111  Expansion of (1 + 3x - 2x^2 - 12x^3)/(1 - 9x^2 + 20x^4).
 * https://oeis.org/A097111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*a(n-2) - 20*a(n-4); a(0)=1, a(1)=3, a(2)=7, a(3)=15
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to support out‑of‑order calls */
    static seq_t *cache = NULL;
    static size_t cap    = 0;       /* number of cached elements */

    size_t idx = (size_t)n;        /* n is non‑negative */

    if (idx >= cap) {              /* need to extend the cache */
        size_t old_cap = cap;
        size_t new_cap = idx + 1;

        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failure – return 0 (undefined) */

        cache = new_cache;
        cap   = new_cap;

        /* compute new elements */
        for (size_t i = old_cap; i <= idx; ++i) {
            seq_t val;
            if (i == 0)        val = 1;
            else if (i == 1)   val = 3;
            else if (i == 2)   val = 7;
            else if (i == 3)   val = 15;
            else
                val = cache[i-2] * 9 - cache[i-4] * 20;   /* recurrence */

            cache[i] = val;
        }
    }

    return cache[idx];
}
