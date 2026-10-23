/*
 * A158884  G.f. A(x) satisfies d/dx x*A(x) = 1+x + x*[d/dx log(A(x))].
 * https://oeis.org/A158884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A158884: a(0)=1; a(n)=a(n-1)-((n+2)/2)*∑_{k=1}^{n-1} a(k)*a(n-k) */
#include <stdlib.h>

static seq_t *cache = NULL;    /* memoised values */
static idx_t  computed = 0;    /* number of terms currently stored (0..computed-1) */

seq_t a(idx_t n)
{
    /* If we don't have all terms up to n, grow the cache and compute them */
    if (n + 1 > computed)
    {
#if defined(__cplusplus)
        seq_t *new_cache = reinterpret_cast<seq_t*>(realloc(cache,
                               (n + 1) * sizeof(seq_t)));
#else
        seq_t *new_cache = (seq_t*)realloc(cache,
                               (n + 1) * sizeof(seq_t));
#endif
        if (!new_cache) return 0;   /* allocation failure – caller should handle */
        cache = new_cache;
        for (idx_t m = computed; m <= n; ++m)
        {
            if (m == 0)        /* initial value */
            {
                cache[0] = (seq_t)1;
                continue;
            }
            /* compute the convolution sum */
            seq_t sum = 0;
            for (idx_t k = 1; k < m; ++k)
                sum += cache[k] * cache[m - k];

            /* (m+2)/2 * sum  ==  ((m+2)*sum)/2  */
            seq_t prod = (seq_t)(m + 2) * sum;
            prod /= 2;                       /* exact division by 2 */

            cache[m] = cache[m - 1] - prod;   /* recurrence */
        }
        computed = n + 1;
    }
    return cache[n];
}
