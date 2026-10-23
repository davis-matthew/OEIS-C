/*
 * A013309  Expansion of e.g.f.:
 *          exp(exp(x)-cos(x))=1+x+3/2!*x^2+8/3!*x^3+29/4!*x^4+112/5!*x^5...
 * https://oeis.org/A013309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>=0, a(n+1)=sum_{i=0}^n binom(n,i)*a(i)*e_{n-i},
   where e_t = 1 if t is even,
            e_t = 2 if t % 4 == 1,
            e_t = 0 if t % 4 == 3. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache        = 0;
    static idx_t cache_size    = 0;
    static idx_t ready         = 0;    /* first index not yet computed */

    /* Grow cache to accommodate index n */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise first element */
    if (ready == 0) {
        cache[0] = 1;
        ready = 1;
    }

    /* Compute subsequent terms up to n if not already done */
    if (n >= ready) {
        for (idx_t m = ready; m <= n; ++m) {
            idx_t m1 = m - 1;
            seq_t sum = 0;
            seq_t bin = 1;    /* binom(m1,0) */

            for (idx_t i = 0; i <= m1; ++i) {
                idx_t t = m1 - i;
                seq_t e;
                if ((t & 1ULL) == 0ULL)
                    e = 1;
                else if ((t % 4ULL) == 1ULL)
                    e = 2;
                else
                    e = 0;

                if (e != 0) {
                    seq_t temp = bin * cache[i];
                    temp *= e;
                    sum += temp;
                }

                /* update binomial for next i */
                if (i < m1) {
                    bin = bin * (seq_t)(m1 - i);
                    bin = bin / (seq_t)(i + 1);
                }
            }
            cache[m] = sum;
        }
        ready = n + 1;
    }

    return cache[n];
}
