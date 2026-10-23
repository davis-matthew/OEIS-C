/*
 * A164069  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -595*a(n-6) + 34*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)),
 *            with initial values a(0)=1, a(1)=36, a(2)=1260, a(3)=44100,
 *            a(4)=1543500, a(5)=54022500, a(6)=1890786870
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* array of computed terms */
    static idx_t cached = 0;       /* number of elements currently cached */

    if (n >= cached) {
        /* Allocate or grow the cache to hold values up to *n* */
        size_t newlen = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* Initial values (may truncate if seq_t is small) */
        static const seq_t init[7] = {
            (seq_t)1,
            (seq_t)36,
            (seq_t)1260,
            (seq_t)44100,
            (seq_t)1543500,
            (seq_t)54022500,
            (seq_t)1890786870
        };

        /* Compute missing terms using the recurrence */
        for (idx_t i = cached; i <= n; ++i) {
            size_t ii = (size_t)i;
            if (i < 7) {
                cache[ii] = init[i];
            } else {
                seq_t sum = 0;
                for (int k = 1; k <= 5; ++k) {
                    sum += cache[ii - k] * (seq_t)34;
                }
                sum -= cache[ii - 6] * (seq_t)595;
                cache[ii] = sum;
            }
        }
        cached = n + 1;
    }
    return cache[(size_t)n];
}
