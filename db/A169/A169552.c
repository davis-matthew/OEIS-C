/*
 * A169552  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n + 9*(a(n-1)+...+a(n-34)) - 45*a(n-35) where p_n = 1
 *            if n==0 or n==35, 2 if 1≤n≤34, otherwise 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc_array.h"

seq_t a(idx_t n)
{
    /* allocate an array large enough for all terms up to n */
    seq_t *a = seq_alloc_array((size_t)n + 1);

    /* base values for 0≤k≤34 */
    a[0] = (seq_t)1;
    if (n >= 1) {
        for (size_t k = 1; k <= n && k <= 34; ++k) {
            seq_t v = (seq_t)1;
            for (size_t t = 0; t < k - 1; ++t) v *= (seq_t)10;   /* 10^(k-1) */
            v *= (seq_t)11;                                    /* 11*10^(k-1) */
            a[k] = v;
        }
    }

    /* recurrence for k≥35 */
    if (n >= 35) {
        for (size_t k = 35; k <= n; ++k) {
            seq_t sum = (seq_t)0;
            for (size_t j = 1; j <= 34; ++j) {
                sum += a[k - j];
            }

            seq_t p = (k == 35 ? (seq_t)1 : 
                       (k <= 34 ? (seq_t)2 : (seq_t)0));

            seq_t term = p + (seq_t)9 * sum - (seq_t)45 * a[k - 35];
            a[k] = term;
        }
    }

    return a[n];
}
