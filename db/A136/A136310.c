/*
 * A136310  a(n)=a(n-1)*(2^K)+ n*(n+1)/2 ; a(0)=1; K=floor(log_2 n*(n+1)/2).
 * https://oeis.org/A136310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)*(2^K) + n*(n+1)/2 ; a(0)=1 where K =
 *            floor(log2(n*(n+1)/2))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* cache stores previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* number of entries currently stored */

    /* ensure cache can hold value for index n */
    if (n >= cache_size) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */

        cache = tmp;
        /* compute entries from cache_size (old size) up to n inclusive */
        if (cache_size == 0) {        /* first call or cache cleared */
            cache[0] = 1;
            cache_size = 1;
        }
        for (idx_t i = cache_size; i <= n; ++i) {
            /* triangular number T = i*(i+1)/2  (fits in idx_t for i up to 10000) */
            idx_t tri = (i * (i + 1ULL)) / 2ULL;
            idx_t K = floor_log2_uint64(tri);          /* K = ⌊log₂T⌋ */
            seq_t pow2K = ((seq_t)1) << K;            /* 2^K  */
            seq_t term = ((seq_t)i * ((seq_t)i + 1)) / 2;  /* n*(n+1)/2 in seq_t */
            cache[i] = cache[i - 1] * pow2K + term;   /* recurrence */
        }
        cache_size = new_cap;
    }

    return cache[n];
}
