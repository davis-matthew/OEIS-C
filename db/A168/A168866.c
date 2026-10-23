/*
 * A168866  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=45, a(2)=1980, ... and for n ≥ 0 the sequence
 *            satisfies the linear recurrence a(n) = 43·∑_{k=1}^{19} a(n−k) –
 *            946·a(n−20) + N(n) where N(0)=1, N(k)=2 (1≤k≤19), N(20)=1 and
 *            N(k)=0 otherwise
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* number of cached entries */

    /* ensure we have at least n+1 entries allocated and computed */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 20;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* fatal error on allocation failure */
        cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i) {
            seq_t sum = 0;
            for (size_t k = 1; k <= 19; ++k) {
                if (i >= k) sum += cache[i - k];
            }
            seq_t val = sum * (seq_t)43;
            if (i >= 20) val -= (seq_t)946 * cache[i - 20];

            /* add numerator contribution */
            if (i == 0) val += (seq_t)1;
            else if (i <= 19) val += (seq_t)2;
            else if (i == 20) val += (seq_t)1;

            cache[i] = val;
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
