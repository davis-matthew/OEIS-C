/*
 * A032824  Numbers whose set of base-12 digits is {1,4}.
 * https://oeis.org/A032824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032824: Numbers whose set of base‑12 digits is {1,4}
   a(1)=1, a(2)=4;
   for n≥3:  a(n) = 12⋅a(floor((n-1)/2)) + (n odd ? 1 : 4)        */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static idx_t  computed_max = 0;  /* largest index computed so far */

    /* ensure we have room for indices up to n */
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap <= (size_t)n + 1) new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* initialise the first two values if not done yet */
    if (computed_max == 0) {
        if (cap < 3) {  /* must hold at least indices 1 and 2 */
            cache = (seq_t *)realloc(cache, 3 * sizeof(seq_t));
            cap = 3;
        }
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        computed_max = 2;
    }

    /* compute any missing values up to n */
    if (computed_max < n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            if (i & 1U) {                          /* odd i */
                cache[(size_t)i] =
                    (seq_t)12 * cache[(size_t)(i >> 1)] + (seq_t)1;
            } else {                                /* even i */
                cache[(size_t)i] =
                    (seq_t)12 * cache[(size_t)((i - 2) >> 1)] + (seq_t)4;
            }
        }
        computed_max = n;
    }

    return cache[(size_t)n];
}
