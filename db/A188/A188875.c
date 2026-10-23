/*
 * A188875  Number of 3Xn binary arrays without the pattern 0 0 0
 *          antidiagonally or horizontally.
 * https://oeis.org/A188875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +10*a(n-2) +41*a(n-3) -3*a(n-4) -10*a(n-5)
 *            -124*a(n-6) * +96*a(n-7) -50*a(n-8) +71*a(n-9) -43*a(n-10)
 *            +14*a(n-11) -5*a(n-12) +a(n-13) * Initial values (n = 1 … 13): *
 *            8, 64, 316, 1901, 11332, 65656, 385700, 2262261, 13249261,
 *            77665756, * 455194656, 2667688716, 15634970127 * * The...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static state: memoised cache of computed terms  */
    static seq_t *cache = NULL;
    static idx_t cached = 0;          /* number of terms stored so far */

    /* ----- initialise cache with known first 13 terms ----- */
    if (cache == NULL) {
        cached = 13;
        cache = (seq_t *)malloc(sizeof(seq_t) * cached);
        if (cache == NULL) return 0;  /* allocation failure */

        static const seq_t init[13] = {
            8ULL, 64ULL, 316ULL, 1901ULL, 11332ULL, 65656ULL,
            385700ULL, 2262261ULL, 13249261ULL, 77665756ULL,
            455194656ULL, 2667688716ULL, 15634970127ULL
        };

        for (idx_t i = 0; i < cached; ++i)
            cache[i] = init[i];
    }

    /* ----- extend cache if requested index is beyond current size ----- */
    if (n > cached) {
        /* allocate space for terms 1 … n (n is 1‑based) */
        size_t new_size = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) return 0;  /* allocation failure */
        cache = new_cache;

        /* recurrence coefficients */
        const int coeffs[13] = {3, 10, 41, -3, -10, -124,
                                96, -50, 71, -43, 14, -5, 1};

        /* compute each new term using the recurrence */
        for (idx_t i = cached + 1; i <= n; ++i) {
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;

            for (int k = 0; k < 13; ++k) {
                /* term a(i-k-1) corresponds to cache[i-k-2] (0‑based) */
                idx_t idx = i - k - 2;
                seq_t term = cache[idx];

                if (coeffs[k] >= 0)
                    pos_sum += term * (seq_t)coeffs[k];
                else
                    neg_sum += term * (seq_t)(-coeffs[k]);
            }

            /* final value = positive part – negative part */
            cache[i - 1] = pos_sum - neg_sum;
        }

        cached = n;
    }

    /* n ≥ 1: return the 0‑based cache entry */
    return cache[n - 1];
}
