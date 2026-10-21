/*
 * A210177  Number of (n+1)X3 0..3 arrays containing all values 0..3 with
 *          every 2X2 subblock having three or four distinct values, and new
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A210177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A210177  -  OEIS sequence
    Formula (for n ≥ 9):
        a(n) = 39*a(n-1) -211*a(n-2) -523*a(n-3) +2954*a(n-4)
             -1030*a(n-5) -4272*a(n-6) +2096*a(n-7) -192*a(n-8)
    Initial values:
        a(1)=51, a(2)=2316, a(3)=79017, a(4)=2563795,
        a(5)=82255351, a(6)=2632469015, a(7)=84200356757,
        a(8)=2692825679001
*/

#include <stdlib.h>        /* malloc, free, size_t */

seq_t a(idx_t n)
{
    static seq_t *cache        = NULL; /* array index 1 … */
    static idx_t  cache_len   = 0;     /* allocated length */
    static idx_t  computed_max = 0;    /* largest n that has been computed */

    /* behaviour undefined for n < 1 */
    if (n < 1) return 0;

    /* grow cache if needed */
    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *new_cache = (seq_t *)malloc((new_len + 1) * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure */

        /* copy old data */
        if (cache)
            for (idx_t i = 1; i <= cache_len; ++i)
                new_cache[i] = cache[i];

        free(cache);
        cache      = new_cache;
        cache_len  = new_len;
    }

    /* initialise first eight terms once */
    if (computed_max < 8) {
        static const seq_t init[9] = {
            0ULL,                /* unused 0‑th index */
            51ULL,               /* a(1) */
            2316ULL,             /* a(2) */
            79017ULL,            /* a(3) */
            2563795ULL,          /* a(4) */
            82255351ULL,         /* a(5) */
            2632469015ULL,       /* a(6) */
            84200356757ULL,      /* a(7) */
            2692825679001ULL     /* a(8) */
        };
        idx_t start = computed_max + 1;
        if (start < 1) start = 1;
        for (idx_t i = start; i <= 8 && i <= n; ++i)
            cache[i] = init[i];
        if (computed_max < 8 && n >= 8)
            computed_max = 8;
        else if (computed_max < n)
            computed_max = start - 1;
    }

    /* compute remaining terms up to n */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        /* recurrence only applies for k >= 9; for k <= 8 we already set */
        if (k <= 8) continue;

        /* use signed intermediate if available; otherwise
           rely on unsigned arithmetic with careful ordering        */
        seq_t pos = 39ULL  * cache[k-1]
                  + 2954ULL * cache[k-4]
                  + 2096ULL * cache[k-7];

        seq_t neg = 211ULL  * cache[k-2]
                  + 523ULL  * cache[k-3]
                  + 1030ULL * cache[k-5]
                  + 4272ULL * cache[k-6]
                  + 192ULL  * cache[k-8];

        cache[k] = pos - neg;   /* guaranteed non‑negative */
    }

    computed_max = n;
    return cache[n];
}
