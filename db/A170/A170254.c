/*
 * A170254  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=5, a(n)=4*a(n-1) for 2≤n≤40, a(41) = (5 -
 *            3*Σ_{k=1}^{40}a(k))/3, a(n) = 2*a(n-41) - Σ_{k=1}^{40}a(n-k) for
 *            n>41
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;          /* number of stored terms */

    /* extend cache if necessary */
    if (n + 1 > cached_len) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) {
            /* allocation failure – abort silently (could handle differently) */
            return (seq_t)0;
        }
        cache = tmp;
        cached_len = n + 1;
    }

    /* compute terms that are not yet known */
    for (idx_t i = cached_len - 1; i < n + 1; ++i) {
        /* note: cached_len may be 0, so i will start at 0 */
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)5;
        } else if (i <= 40) {
            cache[i] = cache[i - 1] * (seq_t)4;
        } else if (i == 41) {
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= 40; ++j)
                sum += cache[j];
            cache[41] = ((seq_t)5 - (seq_t)3 * sum) / (seq_t)3;
        } else { /* i > 41 */
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= 40; ++j)
                sum += cache[i - j];
            cache[i] = cache[i - 41] * (seq_t)2 - sum;
        }
    }

    return cache[n];
}
