/*
 * A173090  a(1)=1, a(2)=2, a(n)=a(n-1)+floor(a(n-2)*a(n-1)/(a(n-2)+a(n-1))).
 * https://oeis.org/A173090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(n)=a(n-1)+floor(a(n-2)*a(n-1)/(a(n-2)+a(n-1)))
 *            for n≥3
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values; 1‑based indexing in the specification,
       zero‑based array indexing in C. */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;

    if (n == 0) /* undefined; return zero for safety */
        return 0;

    /* Extend the cache if necessary */
    if (n > max_n) {
        idx_t new_max = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_max * sizeof(seq_t));
        if (!new_cache) { /* allocation failed */
            if (cache) free(cache);
            cache = NULL;
            max_n = 0;
            return 0;
        }
        cache = new_cache;

        /* Initialize new entries */
        for (idx_t i = max_n + 1; i <= new_max; ++i) {
            if (i == 1)
                cache[0] = 1;
            else if (i == 2)
                cache[1] = 2;
            else {
                seq_t a_prev2 = cache[i - 3]; /* a(i-2) */
                seq_t a_prev1 = cache[i - 2]; /* a(i-1) */
                seq_t product = a_prev2 * a_prev1;
                seq_t sum     = a_prev2 + a_prev1;
                seq_t increment = product / sum;  /* truncating division = floor for positives */
                cache[i - 1] = a_prev1 + increment;
            }
        }
        max_n = new_max;
    }
    return cache[n - 1];
}
