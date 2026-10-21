/*
 * A211260  Number of (n+1)X(n+1) -6..6 symmetric matrices with every 2X2
 *          subblock having sum zero and two, three or four distinct values.
 * https://oeis.org/A211260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical 14‑term recurrence a(n) = 44*a(n-1) - 834*a(n-2) +
 *            8822*a(n-3) - 56045*a(n-4) + 210804*a(n-5) - 405935*a(n-6) +
 *            122980*a(n-7) + 727296*a(n-8) - 286484*a(n-9) - 957331*a(n-10) -
 *            589182*a(n-11) - 162870*a(n-12) - 21384*a(n-13) - 1080*a(n-14)
 *            initial values: a(1)=84, a(2)=546, a(3)=3552, a...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[14] = {
    84ULL, 546ULL, 3552ULL, 23154ULL, 151218ULL,
    989484ULL, 6486618ULL, 42598386ULL, 280208436ULL,
    1845953430ULL, 12177085710ULL, 80422381488ULL,
    531677196078ULL, 3517904882982ULL
};

static const int coeffs[14] = {
    44, -834, 8822, -56045, 210804,
    -405935, 122980, 727296, -286484, -957331,
    -589182, -162870, -21384, -1080
};

static seq_t *cache = NULL;
static size_t cached_n = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but return 0 */

    /* Grow cache if necessary */
    if (cached_n < (size_t)n) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = cache;
        if (cache == NULL) {
            new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        } else {
            new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (!new_cache) exit(1); /* allocation failure */
        cache = new_cache;

        /* Compute new terms */
        for (idx_t i = (idx_t)(cached_n + 1); i <= n; ++i) {
            size_t idx = (size_t)(i - 1);
            if (i <= (idx_t)14) {               /* initial values */
                cache[idx] = init_vals[idx];
            } else {
                seq_t pos = 0;
                seq_t neg = 0;
                idx_t m = i - 1;                /* zero‑based index of a(i) */
                for (int j = 0; j < 14; ++j) {
                    int c = coeffs[j];
                    size_t prev_idx = (size_t)(m - (j + 1)); /* a(i-j-1) */
                    seq_t term = cache[prev_idx];
                    if (c > 0) {
                        pos += term * (seq_t)c;
                    } else {
                        neg += term * (seq_t)(-c);
                    }
                }
                cache[idx] = pos - neg;          /* guaranteed non‑negative */
            }
        }
        cached_n = (size_t)n;
    }
    return cache[n - 1];
}
