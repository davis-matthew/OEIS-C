/*
 * A237230  Number of (n+1)X(4+1) 0..2 arrays with the maximum plus the
 *          minimum of every 2X2 subblock differing from its horizontal and
 *          vertical neighbors by exactly one.
 * https://oeis.org/A237230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n−2) + 3*a(n−3) + 24*a(n−4) + 21*a(n−5) + 28*a(n−6) −
 *            64*a(n−7) − 199*a(n−8) − 396*a(n−9) − 578*a(n−10) − 30*a(n−11) −
 *            136*a(n−12) − 192*a(n−13) for n > 16 Initial terms (a(1)…a(16))
 *            are taken directly from the OEIS entry
 * Range:     u8 none, u16 n <= 3, u32 n <= 14, u64 n <= 35, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[17] = {
    0ULL,          /* dummy for index 0 */
    2784ULL,      /* a(1) */
    10888ULL,     /* a(2) */
    29456ULL,     /* a(3) */
    83436ULL,     /* a(4) */
    233536ULL,    /* a(5) */
    671080ULL,    /* a(6) */
    1919288ULL,   /* a(7) */
    5542856ULL,   /* a(8) */
    15833748ULL,  /* a(9) */
    45309424ULL,  /* a(10) */
    129364012ULL, /* a(11) */
    371396844ULL, /* a(12) */
    1063000280ULL,/* a(13) */
    3048786056ULL,/* a(14) */
    8719902452ULL,/* a(15) */
    24994666460ULL/* a(16) */
};

seq_t a(idx_t n)
{
    /* Cache for computed terms (1‑based indexing). */
    static seq_t *cache = NULL;
    static idx_t cache_alloc = 0;   /* allocated size (number of slots) */
    static idx_t max_comp  = 0;     /* highest index whose value is known */

    /* Ensure cache large enough to hold indices up to n (inclusive). */
    if (cache_alloc <= n) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – return 0 */
        cache = tmp;
        memset((void *)(cache + cache_alloc), 0, (n + 1 - cache_alloc) * sizeof(seq_t));
        cache_alloc = n + 1;

        /* initialise known initial terms if they were not already set */
        for (idx_t i = 1; i <= 16 && i < cache_alloc; ++i)
            cache[i] = init[i];
    }

    /* Return a direct lookup for n ≤ 16 */
    if (n <= 16) return cache[n];

    /* Grow computed terms up to n */
    if (max_comp < n) {
        /* start from one beyond last known value or from 17 if none computed yet */
        idx_t start = (max_comp >= 17) ? max_comp + 1 : 17;
        for (idx_t i = start; i <= n; ++i) {
            seq_t val = 0;
            /* coefficient array: index 0 → a(i‑2), …, index 12 → a(i‑13) */
            static const int coeff[13] = {4, 3, 24, 21, 28, -64, -199, -396, -578, -30, -136, -192};
            for (int k = 0; k < 13; ++k) {
                idx_t j = i - (idx_t)(k + 2);
                seq_t t = cache[j];
                int c = coeff[k];
                if (c >= 0)
                    val += t * (seq_t)c;
                else
                    val -= t * (seq_t)(-c);
            }
            cache[i] = val;
        }
        max_comp = n;
    }

    return cache[n];
}
