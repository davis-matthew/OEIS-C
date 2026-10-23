/*
 * A170485  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by the generating function (t^45 + 2*t^44 + ... +
 *            2*t + 1)/(903*t^45 - 42*t^44 - ... - 42*t + 1). Recurrence
 *            (valid for all n ≥ 45): a(n) = 42*(a(n-1)+…+a(n-44)) -
 *            903*a(n-45) For 0 ≤ n ≤ 44: a(n) = coeff_N(n) +
 *            42*(a(n-1)+…+a(n-44)), where coeff_N(0)=1 and coeff_N(1..44)=2.
 *            Cached v...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0..len-1] holds computed terms */
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of terms actually stored */

    /* initialise on first call */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        /* a(0) = 1 */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* grow cache to at least n+1 terms */
    if ((size_t)n >= cache_len) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap < needed)
                new_cap <<= 1;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            cache_cap = new_cap;
        }
        /* compute missing terms */
        for (size_t idx = cache_len; idx <= (size_t)n; ++idx) {
            seq_t term;
            if (idx <= 44) { /* use coefficients from numerator */
                seq_t coeff = (idx == 0) ? (seq_t)1 : (seq_t)2;
                term = coeff;
                /* add 42 * sum of previous up to 44 terms */
                size_t upto = idx < 44 ? idx : 44;
                for (size_t i = 1; i <= upto; ++i)
                    term += (seq_t)42 * cache[idx - i];
            } else {        /* recurrence for n ≥ 45 */
                term = 0;
                for (size_t i = 1; i <= 44; ++i)
                    term += (seq_t)42 * cache[idx - i];
                term -= (seq_t)903 * cache[idx - 45];
            }
            cache[idx] = term;
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
