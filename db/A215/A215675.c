/*
 * A215675  a(1) = 1, a(n) = 2 if 1<n<=3, a(2n+1) = a(n)+1, a(2n+2) =
 *          a(n)+a(n+1)+1 otherwise.
 * https://oeis.org/A215675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=a(3)=2; Recurrence: a(2n+1)=a(n)+1 for n≥1,
 *            a(2n+2)=a(n)+a(n+1)+1 for n≥1
 * Range:     u8 n <= 1193, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A simple memoised implementation that builds the sequence on demand.
   It uses a static cache that is expanded as needed.  The cache array
   is indexed from 0 (unused) to n.  All indices ≥1 receive a value. */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* seq_t and idx_t are defined by the surrounding build.  */

seq_t a(idx_t n)
{
    /* Cache of already-computed values.  */
    static seq_t *cache      = NULL;   /* array of length cache_cap  */
    static size_t cache_cap   = 0;      /* current allocated size     */
    static idx_t  cached_up_to = 0;     /* largest n that has been filled */

    /* a(0) is undefined -- return 0 to avoid UB for accidental use. */
    if (n == 0) return (seq_t)0;

    /* If the requested index is beyond the cache capacity, grow the array. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;   /* indices 0 … n   */

        seq_t *new_cache = alloc_seq_array(new_cap);

        /* Preserve existing pre‑computed values.  */
        if (cache != NULL) {
            for (idx_t i = 0; i <= cached_up_to; ++i) {
                new_cache[i] = cache[i];
            }
        }

        /* Release the old array, if any. */
        if (cache != NULL) free_seq_array(cache);

        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2 || i == 3) {
            cache[i] = (seq_t)2;
        } else if (i & 1ULL) {                /* odd: a(2k+1) = a(k)+1 */
            idx_t k = i >> 1;                 /*   k = floor(i/2)      */
            cache[i] = cache[k] + (seq_t)1;
        } else {                              /* even: a(2k+2) = a(k)+a(k+1)+1 */
            idx_t k = (i >> 1) - 1;            /*   k = i/2 - 1          */
            cache[i] = cache[k] + cache[k + 1] + (seq_t)1;
        }
    }

    cached_up_to = n;
    return cache[n];
}
