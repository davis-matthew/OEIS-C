/*
 * A154223  Diagonal sums of number triangle A154221.
 * https://oeis.org/A154223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Diagonal sums of number triangle A154221 Recurrence: a(n) =
 *            3*a(n-1) + a(n-2) - 9*a(n-3) + 4*a(n-4) + 6*a(n-5) - 4*a(n-6) +
 *            N(n) where N(n) are the first seven coefficients of the
 *            numerator of the generating function, namely N(0)= 1, N(1)=-2,
 *            N(2)=-2, N(3)= 5, N(4)=-1, N(5)=-1, N(6)= 1. For n≥7 t...
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int N_coeff[7] = {1, -2, -2, 5, -1, -1, 1};

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of cached entries */

    /* Allocate cache if not done yet */
    if (cache_len == 0) {
        cache_len = 0;
        cache = NULL;
    }

    /* Grow cache to accommodate index n (if necessary) */
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure; unlikely */
        cache = tmp;

        /* Compute missing values from current cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t pos = 0;   /* sum of terms with positive coefficient */
            seq_t neg = 0;   /* sum of terms with negative coefficient */

            /* Positive contributions */
            if (i >= 1) pos += (seq_t)3 * cache[i - 1];
            if (i >= 2) pos += cache[i - 2];
            if (i >= 4) pos += (seq_t)4 * cache[i - 4];
            if (i >= 5) pos += (seq_t)6 * cache[i - 5];
            if (i <= 6 && N_coeff[i] > 0) pos += (seq_t)N_coeff[i];

            /* Negative contributions */
            if (i >= 3) neg += (seq_t)9 * cache[i - 3];
            if (i >= 6) neg += (seq_t)4 * cache[i - 6];
            if (i <= 6 && N_coeff[i] < 0) neg += (seq_t)(-N_coeff[i]);

            /* Result = positive - negative (non‑negative guarantee) */
            cache[i] = pos - neg;
        }
        cache_len = new_len;
    }

    return cache[n];
}
