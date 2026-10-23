/*
 * A123188  a(n) = -5*a(n-1) + 8*a(n-2) + 6*a(n-3) - 4*a(n-4).
 * https://oeis.org/A123188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -5*a(n-1) + 8*a(n-2) + 6*a(n-3) - 4*a(n-4) Offset: 1
 *            (a(1)=1, a(2)=0, a(3)=2, a(4)=-8)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 14, i64 n <= 26, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* behaviour below offset is irrelevant */

    static seq_t *cache = NULL;
    static idx_t   size  = 0;   /* number of terms stored in cache (>=1) */

    /* initialise cache with first four terms if not already done */
    if (size < 4) {
        size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * size);
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)0;   /* a(2) */
        cache[2] = (seq_t)2;   /* a(3) */
        cache[3] = (seq_t)-8;  /* a(4) */
    }

    /* return cached value if already computed */
    if (n <= size) return cache[n-1];

    /* grow cache to accommodate term n */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)n);
    if (!new_cache) return 0;           /* allocation failure: fall back to 0 */
    cache = new_cache;

    /* compute new terms iteratively */
    for (idx_t i = size + 1; i <= n; ++i) {
        cache[i-1] =   (-(seq_t)5 * cache[i-2])
                      + ((seq_t)8 * cache[i-3])
                      + ((seq_t)6 * cache[i-4])
                      - ((seq_t)4 * cache[i-5]);
    }

    size = n;
    return cache[n-1];
}
