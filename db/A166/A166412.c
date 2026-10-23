/*
 * A166412  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 16 * (a(n-1)+a(n-2)+ ... + a(n-10)) - 136 *
 *            a(n-11) for n ≥ 12, a(11) = 16 * (a(10)+a(9)+ ... + a(1)) - 136
 *            * a(0) + 1 (the +1 term comes from the numerator of the
 *            generating function); the values for n = 1..10 follow from the
 *            same recurrence with the appropriate +2 constant, whi...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;
    static int initialized = 0;

    /* initial 11 terms a(0)..a(10) */
    static const seq_t init[11] = {
        (seq_t)1u,                        /* a(0) */
        (seq_t)18u,                       /* a(1) */
        (seq_t)306u,                      /* a(2) */
        (seq_t)5202u,                     /* a(3) */
        (seq_t)88434u,                    /* a(4) */
        (seq_t)1503378u,                  /* a(5) */
        (seq_t)25557426u,                 /* a(6) */
        (seq_t)434476242u,                /* a(7) */
        (seq_t)7386096114ull,             /* a(8) */
        (seq_t)125563633938ull,           /* a(9) */
        (seq_t)2134581776946ull           /* a(10) */
    };

    /* First time: allocate storage up to n and copy known values */
    if (!initialized) {
        size_t need = (size_t)(n > 10 ? n : 10) + 1;
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = init[i];
        max_cached = 10;
        initialized = 1;

        if (n <= 10)
            return cache[n];
    }

    /* Grow the cache if necessary and compute new terms */
    if (n > max_cached) {
        size_t newsize = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        cache = tmp;

        for (idx_t k = max_cached + 1; k <= n; ++k) {
            /* sum of the previous 10 terms */
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= 10; ++j)
                sum += cache[k - j];

            if (k == 11) {
                /* special case: +1 from the numerator term */
                cache[k] = (seq_t)16 * sum - (seq_t)136 * cache[0] + (seq_t)1;
            } else {
                cache[k] = (seq_t)16 * sum - (seq_t)136 * cache[k - 11];
            }
        }
        max_cached = n;
    }

    return cache[n];
}
