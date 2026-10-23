/*
 * A111465  a(1) = 1, a(n+1) = a(n)/T(n+1), if T(n+1) divides a(n), else
 *          a(n+1) = a(n) *T(n+1), where T(n) = n*(n+1)/2 is a triangular
 *          number (A000217).
 * https://oeis.org/A111465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1; a(n+1) = a(n)/T(n+1) if T(n+1) divides a(n), a(n+1) =
 *            a(n)*T(n+1) otherwise; T(k) = k*(k+1)/2
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 15, u64 n <= 41, u128 n <= 64,
 *            big any n
 * Helpers:   mul128, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/mul128.h"

seq_t a(idx_t n)
{
    /* Memoised table: cache[0] = a(1) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of terms stored (a(1)..a(cache_size)) */
    static size_t cache_cap  = 0; /* allocated capacity in seq_t elements */

    if (n == 0)            /* undefined, return 0 for safety */
        return (seq_t)0;

    /* first call: initialise cache with a(1) */
    if (cache == NULL) {
        cache_cap  = (size_t)(n > 4 ? n : 4);
        cache      = seq_malloc(cache_cap);
        if (cache == NULL)
            exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;   /* a(1) */
        cache_size = 1;
    }

    /* grow capacity if necessary */
    if (n > (idx_t)cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = seq_realloc(cache, new_cap);
        if (tmp == NULL)
            exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n > cache_size) {
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            seq_t prev = cache[k - 2];                     /* a(k-1)   */
            idx_t tri_idx = k * (k + 1) / 2;               /* T(k)     */
            seq_t tri = (seq_t)tri_idx;                    /* cast to seq_t */

            if ((prev % tri) == 0)
                cache[k - 1] = prev / tri;                 /* a(k) = a(k-1)/T(k) */
            else
                cache[k - 1] = mul128(prev, tri);         /* a(k) = a(k-1)*T(k) */
        }
        cache_size = n;
    }

    return cache[n - 1];
}
