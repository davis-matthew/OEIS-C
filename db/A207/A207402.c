/*
 * A207402  Number of n X 7 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8) with offsets a(1)=20, a(2)=400,
 *            a(3)=2340, a(4)=8910, a(5)=26676, a(6)=68060, a(7)=154580,
 *            a(8)=321110
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 37, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)            /* offset is 1; undefined behaviour below, but just in case */
        return (seq_t)0;

    /* static cache for values already computed */
    static seq_t *cache      = NULL;
    static idx_t  capacity  = 0;   /* size of allocated array (indices 0 .. capacity-1) */
    static idx_t  maxi      = 0;   /* maximum n already computed */

    /* ---------- grow cache if necessary ---------- */
    if (n >= capacity) {
        idx_t new_cap = n + 1;     /* indices 0 .. n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp)                   /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        capacity = new_cap;
    }

    /* ---------- fill initial terms if not yet in cache ----------- */
    if (maxi < 8) {
        /* index 0 is unused, just a placeholder */
        static const seq_t init[9] = {0,
            20,    /* a(1) */
            400,   /* a(2) */
            2340,  /* a(3) */
            8910,  /* a(4) */
            26676, /* a(5) */
            68060, /* a(6) */
            154580,/* a(7) */
            321110 /* a(8) */
        };
        /* store only up to the requested n or 8
           whichever is smaller, but do not overwrite existing values */
        idx_t limit = (n < 8) ? n : 8;
        for (idx_t i = maxi + 1; i <= limit; ++i)
            cache[i] = init[i];
        maxi = limit;
    }

    /* ---------- compute remaining terms via recurrence ---------- */
    for (idx_t i = maxi + 1; i <= n; ++i) {
        seq_t val = (seq_t)8  * cache[i-1];
        val -= (seq_t)28 * cache[i-2];
        val += (seq_t)56 * cache[i-3];
        val -= (seq_t)70 * cache[i-4];
        val += (seq_t)56 * cache[i-5];
        val -= (seq_t)28 * cache[i-6];
        val += (seq_t)8  * cache[i-7];
        val -=               cache[i-8];
        cache[i] = val;
    }

    maxi = n;          /* remember up to which n we have computed */
    return cache[n];
}
