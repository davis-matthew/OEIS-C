/*
 * A168717  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^17 = I.
 * https://oeis.org/A168717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 * a(0) = 1
 * For n ≥ 1:
 *   a(n) = 38 * (a(n-1)+a(n-2)+…+a(n-16))
 *          – 741 * a(n-17)
 *          + c(n)
 * where
 *   c(0) = 1,
 *   c(1)…c(16) = 2,
 *   c(17) = 1,
 *   c(n)   = 0 for n ≥ 18.
 * This is the linear recurrence implied by the generating function of
 * sequence A168717.
 */

#include <stdlib.h>

/* Memoised computation of a(n). */
seq_t a(idx_t n)
{
    /* Cache for values a(0)..a(current). */
    static seq_t *cache = NULL;
    static idx_t  size  = 0;           /* number of elements stored */

    if (n >= size) {
        /* Grow the cache to length n+1. */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();             /* allocation failed */
        cache = tmp;

        /* Compute new terms. */
        for (idx_t i = size; i <= n; ++i) {
            seq_t sum = 0;
            /* Sum of previous 16 terms, if available. */
            for (idx_t k = 1; k <= 16; ++k)
                if (i >= k) sum += cache[i - k];

            seq_t val = sum * 38;      /* 38 * sum_{k=1}^{16} a(n-k) */

            if (i >= 17)               /* –741 * a(n-17) */
                val -= cache[i - 17] * 741;

            /* Add the inhomogeneous part c(i). */
            if (i == 0)
                val += 1;
            else if (i <= 16)
                val += 2;
            else if (i == 17)
                val += 1;

            cache[i] = val;
        }
        size = new_size;
    }
    return cache[n];
}
