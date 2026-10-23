/*
 * A169247  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   allocate_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence can be computed from the rational generating function
   G(t) = (t^28 + 2 t^27 + … + 2 t + 1) / (820 t^28 − 40 t^27 − … − 40 t + 1).
   The coefficients a(n) satisfy
      a(0) = 1
      for n ≥ 1:
          a(n) = d(n) + 40·∑_{k=1}^{min(27,n)} a(n−k) − 820·[n≥28]·a(n−28)
   where d(n) = 1 if n = 0 or 28, 2 if 1 ≤ n ≤ 27, otherwise 0.
   (The term [n≥28] is 1 if n≥28 and 0 otherwise.) */

#include <stdlib.h>
#include "../utils/allocate_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n) {
    /* Static cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity                  */
    static idx_t  len   = 0;   /* number of computed values          */

    /* Ensure the cache is large enough */
    if ((idx_t)cap < n + 1) {
        size_t new_cap = n + 1;
        if (new_cap < 64) new_cap = 64;            /* small initial growth            */
        seq_t *new_cache = allocate_seq_array(new_cap);
        if (!new_cache) abort();
        if (cache) {
            /* copy existing values */
            for (size_t i = 0; i < cap; ++i) new_cache[i] = cache[i];
            free_seq_array(cache);
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = len; i <= n; ++i) {
        seq_t d;
        if (i == 0 || i == 28)          d = (seq_t)1;
        else if (i <= 27)               d = (seq_t)2;
        else                            d = (seq_t)0;

        seq_t sum = (seq_t)0;
        /* sum of previous 1..27 terms, multiplied by 40 */
        for (idx_t k = 1; k <= 27 && k <= i; ++k) {
            sum += (seq_t)40 * cache[i - k];
        }
        /* subtract 820 * a(n-28) for n ≥ 28 */
        if (i >= 28) {
            sum -= (seq_t)820 * cache[i - 28];
        }
        cache[i] = d + sum;
    }
    len = n + 1;
    return cache[(size_t)n];
}
