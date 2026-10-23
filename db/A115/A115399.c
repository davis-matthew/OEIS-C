/*
 * A115399  Expansion of c(x^2-x^3), c(x) the g.f. of A000108.
 * https://oeis.org/A115399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 29, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   alloc_seq_array, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0, a(2)=-1, a(3)=1, a(4)=2.
   For n>=5: (n+2)*a(n)+(-n-2)*a(n-1)+4*(n-1)*a(n-2)+
             2*(-4*n+7)*a(n-3)+2*(2*n-5)*a(n-4)=0 */
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/ensure_cache.h"

static const seq_t init_vals[5] = {1, 0, -1, 1, 2};

static seq_t *cache = NULL;      /* pointer to the memoisation array */
static size_t cache_cap = 0;     /* allocated capacity (number of seq_t slots) */
static size_t cache_len = 0;     /* first unused index (number of computed terms) */

/* compute a(n) with memoisation and the 5‑term recurrence */
seq_t a(idx_t n)
{
    /* small indices are handled directly */
    if (n < 5) return init_vals[n];

    /* allocate the cache on the first call */
    if (cache == NULL) {
        cache = alloc_seq_array(5);
        if (!cache) return 0;     /* allocation failure sentinel */
        for (size_t i = 0; i < 5; ++i) cache[i] = init_vals[i];
        cache_cap = 5;
        cache_len = 5;
    }

    /* ensure we have room for a(n) */
    if ((size_t)n >= cache_cap - 1) {
        ensure_cache(n, &cache, &cache_cap);
    }

    /* compute any missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* recurrence derived from the OEIS formula */
        seq_t term1 = (seq_t)((i + 2)) * cache[i - 1];
        seq_t term2 = -(seq_t)(4 * (i - 1)) * cache[i - 2];
        seq_t term3 = (seq_t)(8 * i - 14) * cache[i - 3];
        seq_t term4 = -(seq_t)(2 * (2 * i - 5)) * cache[i - 4];

        seq_t numerator   = term1 + term2 + term3 + term4;
        seq_t denominator = (seq_t)(i + 2);

        cache[i] = numerator / denominator;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
