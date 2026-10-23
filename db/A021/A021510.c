/*
 * A021510  Decimal expansion of 1/506.
 * https://oeis.org/A021510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = a(n-1) - a(n-11) + a(n-12) for n > 12 a(0)=0, a(1)=0,
 *            a(2)=1, a(3)=9, a(4)=7, a(5)=6, a(6)=2, a(7)=8, a(8)=4, a(9)=5,
 *            a(10)=8, a(11)=4, a(12)=9
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /*  Static cache of already computed digits (type uint8_t).             */
    /*  The digits fit in a single byte (0‑9) regardless of the build      */
    /*  target for seq_t, so we can store them efficiently.               */
    /* ------------------------------------------------------------------ */
    static uint8_t *cache = NULL;
    static size_t   cap   = 0;   /* allocation size */
    static size_t   len   = 0;   /* number of stored digits */

    /* ------------------------------------------------------------------ */
    /*  Initial digits (indices 0 … 12).  These are hard‑coded from the    */
    /*  OEIS entry and serve as the base for the recurrence.              */
    /* ------------------------------------------------------------------ */
    static const uint8_t init_vals[13] =
        {0, 0, 1, 9, 7, 6, 2, 8, 4, 5, 8, 4, 9};

    /*  Initialise the cache on first call.                                */
    if (!cache) {
        cap = 256;                     /* start with a modest capacity   */
        cache = (uint8_t*)malloc(cap);
        if (!cache) return (seq_t)0;   /* unlikely, but defensive       */
        len = 13;                      /* we have the first 13 digits    */
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init_vals[i];
    }

    /*  If the requested index is beyond our current cache, grow        */
    /*  the array and compute additional digits by the recurrence.      */
    if ((idx_t)len <= n) {
        size_t target = (size_t)n;

        /*  Grow the allocation if necessary.                           */
        if (target >= cap) {
            size_t new_cap = cap;
            while (new_cap <= target)
                new_cap *= 2;
            uint8_t *tmp = (uint8_t*)realloc(cache, new_cap);
            if (!tmp) return (seq_t)0; /* fallback in the unlikely case */
            cache = tmp;
            cap = new_cap;
        }

        /*  Compute digits from len up to target (inclusive).             */
        for (size_t i = len; i <= target; ++i) {
            int val = (int)cache[i - 1]   /* a(n-1)   */
                     - (int)cache[i - 11] /* a(n-11)  */
                     + (int)cache[i - 12];/* a(n-12) */
            cache[i] = (uint8_t)val;    /* store the digit 0‑9 */
        }
        len = target + 1;
    }

    /*  Return the requested digit as type seq_t.                         */
    return (seq_t)cache[(size_t)n];
}
