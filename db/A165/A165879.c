/*
 * A165879  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coeff(n) + 15*(a(n-1)+…+a(n-9)) – 120*a(n-10) where
 *            coeff(0)=1, coeff(1..9)=2, coeff(10)=1, coeff(k>10)=0. Initial
 *            values: a(0)=1, a(1)=17, a(2)=272, … (OEIS) Recurrence derived
 *            from the rational generating function
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms, grows as needed */
    static seq_t *cache      = NULL;
    static idx_t  cache_cap  = 0;    /* current allocation size   */
    static idx_t  last_computed = 0; /* last index that has been computed */

    /* allocate / enlarge cache if required */
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure – unlikely in tests */
        /* compute a(0) */
        cache[0] = 1;
        last_computed = 0;
    }
    else if (n >= cache_cap) {
        cache_cap = n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) return 0;
    }

    /* compute new terms only if we haven't reached n yet */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t coeff;
        if (i == 0)            coeff = 1;
        else if (i == 10)      coeff = 1;
        else if (i <= 9)       coeff = 2;
        else                    coeff = 0;

        /* sum 15*a(i-k) for k=1..9 where i-k >= 0 */
        seq_t sum = 0;
        int k;
        for (k = 1; k <= 9; ++k) {
            if (i >= (idx_t)k) {
                sum += cache[i - k] * (seq_t)15;
            }
        }

        seq_t val = coeff + sum;

        /* subtract 120*a(i-10) if defined */
        if (i >= (idx_t)10) {
            val -= cache[i - 10] * (seq_t)120;
        }

        cache[i] = val;
    }

    last_computed = n;
    return cache[n];
}
