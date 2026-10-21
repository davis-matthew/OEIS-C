/*
 * A086347  On a 3 X 3 board, number of n-move routes of chess king ending in
 *          a given side square.
 * https://oeis.org/A086347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)+4*a(n-2), a(0)=1, a(1)=5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Compute A086347, the number of n‑move king routes ending in a side square on a 3×3 board. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* stores computed values */
    static size_t cap = 0;             /* capacity of the cache */
    static size_t max = 0;             /* largest index already computed */

    /* Initial allocation and first two terms */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        cache[0] = (seq_t)1;       /* a(0) */
        cache[1] = (seq_t)5;       /* a(1) */
        max = 1;
    }

    /* If n is beyond what we have, extend cache */
    if ((size_t)n > max) {
        size_t need = (size_t)n + 1;
        if (need > cap) {
            size_t newcap = cap;
            while (newcap < need) newcap *= 2;
            cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            cap = newcap;
        }
        for (size_t i = max + 1; i <= (size_t)n; ++i) {
            seq_t val = cache[i-1] + cache[i-2];  /* a(i-1)+a(i-2) */
            val = val * (seq_t)4;                /* 4*(a(i-1)+a(i-2)) */
            cache[i] = val;                      /* store a(i) */
        }
        max = (size_t)n;
    }

    return cache[(size_t)n];
}
