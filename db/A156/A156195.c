/*
 * A156195  a(2n+2) = 6*a(2n+1), a(2n+1) = 6*a(2n) - 5^n*A000108(n), a(0)=1.
 * https://oeis.org/A156195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/catalan.h"

/* Sequence A156195:
   a(0) = 1
   a(2m)   = 6 * a(2m-1)
   a(2m+1) = 6 * a(2m) - 5^m * Catalan(m)   (Catalan(m) = (2m choose m)/(m+1))
   The function is memoised; calls may occur in any order. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached values a(0..cap-1) */
    static idx_t cache_cap = 0;      /* current allocation size */

    /* initialise cache */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;              /* allocation failure */
        cache[0] = (seq_t)1;                /* a(0) = 1 */
    }

    /* enlarge cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure */
        /* initialise newly allocated slots to zero */
        for (idx_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0) continue;        /* already computed */

        if ((i & 1) == 0) {                  /* i even : a(2m)   = 6 * a(2m-1) */
            cache[i] = cache[i-1] * (seq_t)6;
        } else {                             /* i odd  : a(2m+1) = 6*a(2m) - 5^m*C_m */
            idx_t m = (i - 1) >> 1;           /* m = (i-1)/2       */
            seq_t pow5 = (seq_t)1;
            for (idx_t j = 0; j < m; ++j)     /* compute 5^m */
                pow5 *= (seq_t)5;
            seq_t cat = catalan(m);           /* Catalan(m)        */
            cache[i] = cache[i-1] * (seq_t)6 - pow5 * cat;
        }
    }

    return cache[n];
}
