/*
 * A050135  a(1) = 1, a(n) = floor(a(n-1)/2) if this is not among
 *          0,a(1),...,a(n-1); otherwise a(n) = 4*n.
 * https://oeis.org/A050135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS recurrence for sequence A050135:
   a(1) = 1
   For n ≥ 2:
       x = floor(a(n‑1)/2)
       if x ∉ {0, a(1), …, a(n‑1)}   →  a(n) = x
       else                            →  a(n) = 4*n
   The implementation memoises computed terms in a static array and
   checks existence by scanning the already‑computed prefix.  This
   guarantees correct results for any call order. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms: a(1) … a(cache_len) */
    static seq_t *cache = NULL;   /* pointer to array (index 0 = a(1)) */
    static idx_t cache_len = 0;   /* number of terms stored */

    if (n == 0) return (seq_t)0;  /* safety; specification requires n≥1 */

    /* Return cached value if already computed */
    if (n <= cache_len) return cache[n-1];

    /* Grow the cache to hold n terms */
    idx_t new_len = n;
    if (cache == NULL) {
#ifdef __cplusplus
        cache = new seq_t[new_len];
#else
        cache = (seq_t*)malloc(new_len * sizeof(seq_t));
#endif
    } else if (cache_len < new_len) {
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_len];
        for (idx_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
#endif
    }

    /* Initialise the very first term if it has not yet been set */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;   /* a(1) = 1 */
        cache_len = 1;
    }

    /* Compute terms from cache_len+1 up to n inclusive */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev   = cache[i-2];            /* a(i-1)   */
        seq_t cand   = prev >> 1;             /* floor(prev/2) */

        int exists = 0;
        if (cand == (seq_t)0) {
            exists = 1;                      /* 0 is considered present */
        } else {
            for (idx_t j = 0; j < i-1; ++j) {
                if (cache[j] == cand) { exists = 1; break; }
            }
        }

        if (exists)
            cache[i-1] = ((seq_t)4) * ((seq_t)i);
        else
            cache[i-1] = cand;
    }

    cache_len = n;
    return cache[n-1];
}
