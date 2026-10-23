/*
 * A169303  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, for n>0 * a(n) = 48·(a(n-1)+…+a(n-28)) - 1176·a(n-29) +
 *            c(n), * where c(0)=1, c(1..28)=2, c(29)=1, c(n)=0 for n>29. *
 *            This follows from the rational generating function *
 *            (1+2t+…+2t^28+t^29)/(1-48t-…-48t^28+1176t^29)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cap = 0;         /* size of allocated array   */
    static idx_t max_comp = 0;    /* largest index computed   */

    /* grow cache if needed */
    if (n + 1 > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(*cache) * (n + 1));
        if (!tmp) abort();        /* allocation failed */
        cache = tmp;
        cap = n + 1;
    }

    /* initialise the very first element on first use */
    if (max_comp == 0 && n >= 0) {
        cache[0] = (seq_t)1;
        max_comp = 0;
    }

    /* compute any missing values up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;

        /* add 48·a(i-k) for k=1..28, if i>=k */
        for (idx_t k = 1; k <= 28 && i >= k; ++k) {
            sum += (seq_t)48 * cache[i - k];
        }

        /* subtract 1176·a(i-29) if i>=29 */
        if (i >= 29) {
            sum -= (seq_t)1176 * cache[i - 29];
        }

        /* add numerator coefficient c(i) */
        seq_t coeff;
        if (i == 0)       coeff = (seq_t)1;
        else if (i <= 28) coeff = (seq_t)2;
        else if (i == 29) coeff = (seq_t)1;
        else              coeff = (seq_t)0;

        cache[i] = sum + coeff;
    }

    max_comp = n;
    return cache[n];
}
