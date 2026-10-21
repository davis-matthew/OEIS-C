/*
 * A208996  Number of 5-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero and first differences in -n..n.
 * https://oeis.org/A208996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 4*a(n-3) + a(n-4) + 3*a(n-5) -
 *            3*a(n-6) - a(n-7) + 4*a(n-8) - a(n-9) - 2*a(n-10) + a(n-11)
 *            initial values: a(1)=3, a(2)=17, a(3)=59, a(4)=159, a(5)=351,
 *            a(6)=683, a(7)=1207, a(8)=1989, a(9)=3099, a(10)=4623,
 *            a(11)=6647
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* If called with an index less than 1, we return 0.
       The OEIS definition starts at n=1. */
    if (n == 0) return 0;

    /* Constant initial values for n = 1 … 11 */
    static const seq_t init[12] = {
        0,        /* dummy index 0 */
        3,        /* a(1) */
        17,       /* a(2) */
        59,       /* a(3) */
        159,      /* a(4) */
        351,      /* a(5) */
        683,      /* a(6) */
        1207,     /* a(7) */
        1989,     /* a(8) */
        3099,     /* a(9) */
        4623,     /* a(10) */
        6647      /* a(11) */
    };

    /* Static cache to memoise computed values.  */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;   /* highest n for which cache[n] is valid */

    /* Allocate cache if this is the first call.  We keep the array
       large enough so that the first n values and the 11 initial
       constants are all stored. */
    if (cache == NULL) {
        size_t cap = ((size_t)(n > 11 ? n : 11)) + 1; /* +1 for index 0 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – return 0 */
        /* Zero initialize the whole array */
        memset(cache, 0, cap * sizeof(seq_t));
        /* Copy the initial values that we already know */
        size_t i;
        for (i = 1; i <= (size_t)(n > 11 ? n : 11); ++i)
            cache[i] = init[i];
        cache_max = (size_t)(n > 11 ? n : 11);
    }

    /* If we are asked for a larger n, extend the cache and
       compute the missing terms using the recurrence.  */
    if (n > cache_max) {
        size_t new_cap = (size_t)n + 1; /* +1 for index 0 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure – return 0 */
        cache = tmp;
        /* Zero initialise the new part (not strictly required,
           but harmless). */
        memset(cache + cache_max + 1, 0, (new_cap - cache_max - 1) * sizeof(seq_t));

        idx_t i;
        for (i = cache_max + 1; i <= n; ++i) {
            if (i <= 11) {
                cache[i] = init[i];
            } else {
                /* Positive contributions */
                seq_t pos = 2 * cache[i - 1]    /* 2*a(n-1) */
                          + cache[i - 2]          /*  + a(n-2) */
                          + cache[i - 4]          /*  + a(n-4) */
                          + 3 * cache[i - 5]      /*  + 3*a(n-5) */
                          + 4 * cache[i - 8]      /*  + 4*a(n-8) */
                          + cache[i - 11];        /*  + a(n-11) */
                /* Negative contributions */
                seq_t neg = 4 * cache[i - 3]     /* - 4*a(n-3) */
                          + 3 * cache[i - 6]     /* - 3*a(n-6) */
                          + cache[i - 7]         /* - a(n-7) */
                          + cache[i - 9]         /* - a(n-9) */
                          + 2 * cache[i - 10];   /* - 2*a(n-10) */
                /* We believe pos ≥ neg for all n ≥ 12; if not,
                   the behavior is undefined but tests show it holds. */
                cache[i] = pos - neg;
            }
        }
        cache_max = n;
    }

    return cache[n];
}
