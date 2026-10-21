/*
 * A229809  G.f. B(x) satisfies: B(x) = x + 3*A(x)*C(x), where A(x) = x +
 *          2*B(x)*C(x) and C(x) = x + 5*A(x)*B(x).
 * https://oeis.org/A229809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1.
   Let a_k,b_k,c_k be the coefficients of the series
   A(x)=∑_{k≥1} a_k x^k , B(x)=∑_{k≥1} b_k x^k , C(x)=∑_{k≥1} c_k x^k .
   The defining equations B=x+3AC , A=x+2BC , C=x+5AB lead to
      a_k = 2 · Σ_{i=1}^{k-1} b_i · c_{k-i}
      c_k = 5 · Σ_{i=1}^{k-1} a_i · b_{k-i}
      b_k = 3 · Σ_{i=1}^{k-1} a_i · c_{k-i}
   for k≥2, with a_1=b_1=c_1=1.
   The sequence term a(n) equals b_n.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)                /* undefined for n<1; return 0 */
        return (seq_t)0;
    if (n == 1)               /* first term */
        return (seq_t)1;

    /* allocate arrays of length n+1 (index 0 unused) */
    seq_t *a_arr = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    seq_t *b_arr = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    seq_t *c_arr = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!a_arr || !b_arr || !c_arr) {
        /* memory allocation failed */
        free(a_arr); free(b_arr); free(c_arr);
        return (seq_t)0;
    }

    a_arr[0] = b_arr[0] = c_arr[0] = (seq_t)0;
    a_arr[1] = b_arr[1] = c_arr[1] = (seq_t)1;

    for (idx_t k = 2; k <= n; ++k) {
        seq_t sum_bc = (seq_t)0;
        seq_t sum_ab = (seq_t)0;
        seq_t sum_ac = (seq_t)0;

        /* compute a_k */
        for (idx_t i = 1; i < k; ++i)
            sum_bc += b_arr[i] * c_arr[k - i];
        a_arr[k] = sum_bc * (seq_t)2;

        /* compute c_k */
        for (idx_t i = 1; i < k; ++i)
            sum_ab += a_arr[i] * b_arr[k - i];
        c_arr[k] = sum_ab * (seq_t)5;

        /* compute b_k */
        for (idx_t i = 1; i < k; ++i)
            sum_ac += a_arr[i] * c_arr[k - i];
        b_arr[k] = sum_ac * (seq_t)3;
    }

    seq_t result = b_arr[n];

    free(a_arr);
    free(b_arr);
    free(c_arr);
    return result;
}
