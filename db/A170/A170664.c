/*
 * A170664  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A170664
   a(0) = 1
   a(n) = 31 * 30^(n-1)            for 1 ≤ n ≤ 48
   a(n) = 30 * a(n-1) - 29 * a(n-49) for n ≥ 49
   (This follows from the generating‑function recurrence
    435 a(n‑49) – 29∑_{k=1}^{48}a(n‑k)=0.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache      = NULL;   /* buffer holding values */
    static size_t cache_len  = 0;      /* allocated size of buffer */
    static size_t computed   = 0;      /* number of values stored in cache */

    /* enlarge buffer if necessary */
    if ((size_t)n >= computed) {
        size_t new_len = cache_len ? cache_len : 1;
        while (new_len <= (size_t)n) new_len <<= 1;

        seq_t *new_buf = (seq_t*)realloc(cache,
                                         new_len * sizeof(seq_t));
        if (new_buf == NULL) {
            if (cache != NULL) free(cache);
            return 0;   /* out of memory – sequence value not available */
        }
        cache = new_buf;
        cache_len = new_len;

        /* compute missing values */
        for (size_t i = computed; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[i] = 1;
            } else if (i < 49) {
                if (i == 1) {
                    cache[i] = 31;
                } else {
                    /* a(i) = 30 * a(i-1)   (for 2 ≤ i ≤ 48) */
                    cache[i] = cache[i-1] * (seq_t)30;
                }
            } else {
                /* a(i) = 30 * a(i-1) - 29 * a(i-49)   (for i ≥ 49) */
                cache[i] = cache[i-1] * (seq_t)30;
                cache[i] -= cache[i-49] * (seq_t)29;
            }
        }
        computed = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
