/*
 * A168977  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for 1 ≤ n ≤ 22, a(n)=10*∑_{j=0}^{n-1}a(j)+2;
   a(23)=1+10*∑_{j=1}^{22}a(j)−55*a(0);
   for n≥24, a(n)=10*∑_{j=1}^{22}a(n−j)−55*a(n−23). */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* memoised values */
static size_t cached = 0;        /* number of values computed */

seq_t a(idx_t n) {
    /* grow cache if needed */
    if (n >= cached) {
        size_t newsize = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) abort();   /* insufficient memory */
        cache = tmp;
        /* compute newly required entries */
        for (size_t m = cached; m <= newsize - 1; ++m) {
            if (m == 0) {
                cache[0] = 1;
            } else if (m <= 22) {
                seq_t sum = 0;
                for (size_t j = 0; j < m; ++j) sum += cache[j];
                cache[m] = 10 * sum + 2;
            } else if (m == 23) {
                seq_t sum = 0;
                for (size_t j = 1; j <= 22; ++j) sum += cache[j];
                cache[23] = 1 + 10 * sum - 55 * cache[0];
            } else { /* m >= 24 */
                seq_t sum = 0;
                for (size_t j = 1; j <= 22; ++j) sum += cache[m - j];
                cache[m] = 10 * sum - 55 * cache[m - 23];
            }
        }
        cached = newsize;
    }
    return cache[(size_t)n];
}
