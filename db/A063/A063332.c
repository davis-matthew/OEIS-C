/*
 * A063332  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          59 ).
 * https://oeis.org/A063332
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13, with initial
 *            values: a(2) = -1, a(3) = 117, a(4) = 261, a(5) = 406, a(6) =
 *            551, a(7) = 696, a(8) = 841, a(9) = 986, a(10) = 1131, a(11) =
 *            1276, a(12) = 1421, a(13) = 1564
 * Range:     i8 n <= 3, i16 n <= 42, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init[14] = {0,0,-1,117,261,406,551,696,841,986,1131,1276,1421,1564};

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;                    /* below the offset – undefined */

    /* Cache of computed terms */
    static seq_t *cache = NULL;
    static size_t   cache_cap = 0;
    static idx_t    last_computed = 1;      /* we have computed up to 1 */

    /* Allocate and initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;                     /* initial size */
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                /* allocation failure */
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = 0;

        /* copy known initial terms (indices 2..13) */
        for (idx_t i = 2; i <= 13 && (size_t)i < cache_cap; ++i)
            cache[i] = _init[i];

        last_computed = 13;
    }

    /* Grow cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms in ascending order */
    if (n > last_computed) {
        for (idx_t k = last_computed + 1; k <= n; ++k) {
            if (k <= 13)                           /* safety; should already be set */
                cache[k] = _init[k];
            else
                cache[k] = cache[k - 4] + cache[k - 6] - cache[k - 10];
        }
        last_computed = n;
    }

    return cache[n];
}
