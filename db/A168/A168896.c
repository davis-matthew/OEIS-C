/*
 * A168896  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * For 1 <= n <= 20: a(n) = 2 + 25 * sum_{k=1}^{n}
 *            a(n-k) * For n >= 21: a(n) = 25 * sum_{k=1}^{20} a(n-k) - 325 *
 *            a(n-21) * * The values are generated from the rational
 *            generating function * (t^21 + 2*t^20 + … + 2*t + 1)/(1 - 25*t -
 *            … - 25*t^20 + 325*t^21), * which gives the above line...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cur = 0;   /* next index to compute (all indices < cur are ready) */
    static idx_t cap  = 0;  /* allocated size of cache */

    /* initialise cache */
    if (cur == 0) {
        cap  = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* extend cache until it contains a(n) */
    while (cur <= n) {
        idx_t i = cur;
        seq_t val = 0;
        seq_t sum = 0;

        /* compute sum of the last 20 terms (or fewer for small i) */
        idx_t kmax = (i < 20) ? i : 20;
        for (idx_t k = 1; k <= kmax; ++k) {
            sum += cache[i - k];
        }

        if (i == 0) {
            val = (seq_t)1;                     /* a(0) = 1 */
        } else if (i < 21) {
            /* a(n) = 2 + 25 * sum_{k=1}^{n} a(n-k) */
            val = (seq_t)2 + (seq_t)25 * sum;
        } else {
            /* a(n) = 25 * sum_{k=1}^{20} a(n-k) - 325 * a(n-21) */
            val = (seq_t)25 * sum - (seq_t)325 * cache[i - 21];
        }

        cache[i] = val;
        ++cur;

        /* grow the cache if necessary */
        if (cur > cap) {
            cap <<= 1; /* double the capacity */
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
    }

    return cache[n];
}
