/*
 * A170065  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) + 6 * sum_{k=0}^{n-1} a(k) (n<37) a(n) = 6 *
 *            sum_{k=1}^{36} a(n-k) - 21 * a(n-37) + p(n) (n>=37) where
 *            p(0)=1, p(1..36)=2, p(37)=1, p(n)=0 for n>37. Recurrence fifth
 *            degree 37 with integer coefficients
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* stores a(0)..a(max_needed) */
    static seq_t *pref  = NULL;   /* prefix sums: pref[i] = a(0)+...+a(i) */
    static size_t cache_len = 0;  /* number of elements stored (max index+1) */

    if (n >= cache_len) {
        size_t old_len = cache_len;
        size_t new_len = (size_t)n + 1;
        /* allocate new arrays and copy existing data */
        seq_t *new_cache = (seq_t*)calloc(new_len, sizeof(seq_t));
        seq_t *new_pref  = (seq_t*)calloc(new_len, sizeof(seq_t));
        if (!new_cache || !new_pref) return 0; /* memory error, return 0 */
        for (size_t i = 0; i < old_len; ++i) {
            new_cache[i] = cache[i];
            new_pref[i]  = pref[i];
        }
        free(cache);
        free(pref);
        cache = new_cache;
        pref  = new_pref;
        cache_len = new_len;

        /* compute new terms */
        for (size_t i = old_len; i <= (size_t)n; ++i) {
            seq_t val;
            if (i == 0) {                      /* a(0) */
                val = (seq_t)1;
            } else if (i < 37) {                /* 1 ≤ n < 37 */
                seq_t sum = pref[i-1];
                val = (seq_t)2 + (seq_t)6 * sum;
            } else {                            /* n ≥ 37 */
                seq_t sum36 = pref[i-1] - pref[i-37];
                val = (seq_t)6 * sum36 - (seq_t)21 * cache[i-37];
                if (i == 37) val = val + (seq_t)1;  /* p(37)=1 */
            }
            cache[i] = val;
            pref[i]  = (i==0) ? val : pref[i-1] + val;
        }
    }
    return cache[n];
}
