/*
 * A027391  Number of labeled servers of dimension 4.
 * https://oeis.org/A027391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{min(4,n)} binom(n-1,k-1) * λ_k * a(n-k) * with
 *            λ_1=4, λ_2=6, λ_3=8, λ_4=6
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow fast repeated calls in any order */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;        /* number of values stored (>=0) */

    if (n >= cache_size) {
        /* enlarge cache to hold a[0..n] */
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failed */

        /* copy old values */
        for (idx_t i = 0; i < old_size; ++i)
            new_cache[i] = cache[i];

        /* ensure a[0] = 1 */
        new_cache[0] = (seq_t)1;

        /* compute new entries */
        idx_t start = (old_size > 0) ? old_size : 1;
        for (idx_t i = start; i <= n; ++i) {
            seq_t val = 0;

            /* k = 1 : binom(n-1,0) = 1 */
            val += (seq_t)4 * new_cache[i-1];

            if (i >= 2) {
                /* k = 2 : binom(n-1,1) = (n-1) */
                val += (seq_t)6 * (i-1) * new_cache[i-2];
            }
            if (i >= 3) {
                /* k = 3 : binom(n-1,2) = (n-1)(n-2)/2 */
                seq_t t = ((seq_t)(i-1) * (i-2)) / 2;
                val += (seq_t)8 * t * new_cache[i-3];
            }
            if (i >= 4) {
                /* k = 4 : binom(n-1,3) = (n-1)(n-2)(n-3)/6   */
                /* 6 cancels, so we use (n-1)(n-2)(n-3) directly */
                seq_t t = (seq_t)(i-1) * (i-2) * (i-3);
                val += t * new_cache[i-4];
            }

            new_cache[i] = val;
        }

        /* free previous cache and replace */
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
