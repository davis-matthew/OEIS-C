/*
 * A168849  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; For 1≤n<20: a(n)=2 + 26·∑_{j=1}^{n} a(n−j) For n≥20:
 *            a(n)=26·∑_{j=1}^{19} a(n−j) − 351·a(n−20)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate temporary tables for the terms and their prefix sums. */
    size_t sz = (size_t)n + 1;           /* need indices 0…n */
    seq_t *a   = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *ps  = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!a || !ps) {                     /* allocation failed – return 0 */
        free(a); free(ps);
        return (seq_t)0;
    }

    /* Base case */
    a[0] = (seq_t)1;
    ps[0] = a[0];

    /* Iteratively fill the tables */
    for (idx_t i = 1; i <= n; ++i) {
        if (i < 20) {
            /* Sum of all previous terms: ps[i-1] */
            seq_t sum = ps[i-1];
            a[i] = (seq_t)2 + (seq_t)26 * sum;
        } else {
            /* Sum of the last 19 terms: ps[i-1] - ps[i-20] */
            seq_t sum = ps[i-1] - ps[i-20];
            a[i] = (seq_t)26 * sum - (seq_t)351 * a[i-20];
        }
        ps[i] = ps[i-1] + a[i];
    }

    seq_t result = a[n];
    free(a);
    free(ps);
    return result;
}
