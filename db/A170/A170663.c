/*
 * A170663  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, for 1 ≤ n ≤ 48 a(n)=2 + 28 · Σ_{k=0}^{n-1} a(k). For n ≥
 *            49 a(n)= (28 · Σ_{k=n-48}^{n-1} a(k)) – 406 · a(n-49) + [n==49].
 *            The sequence is the coefficients of
 *            (t^49+2t^48+...+2t+1)/(1-28t-...-28t^48+406t^49)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *A = NULL;      /* cache of already computed terms          */
    static idx_t A_size = 0;     /* number of terms stored in A             */

    /* enlarge cache if necessary */
    if (n >= A_size) {
        idx_t old_size = A_size;
        idx_t new_size = n + 1;
        A = (seq_t *)realloc(A, (size_t)new_size * sizeof(seq_t));
        A_size = new_size;

        /* compute missing terms one by one */
        for (idx_t i = old_size; i < A_size; ++i) {
            if (i == 0) {
                A[i] = (seq_t)1;                         /* a(0) */
            } else if (i <= 48) {
                /* a(i) = 2 + 28 * sum_{k=0}^{i-1} a(k)   (i ≥ 1) */
                seq_t sum = 0;
                for (idx_t k = 0; k < i; ++k) sum += A[k];
                A[i] = (seq_t)2 + (seq_t)28 * sum;
            } else { /* i ≥ 49 */
                seq_t p = (i == 49 ? (seq_t)1 : (seq_t)0);   /* numerator coefficient */
                seq_t sum = 0;
                /* sum of the last 48 terms: a(i-48) .. a(i-1) */
                for (idx_t k = i - 48; k <= i - 1; ++k) sum += A[k];
                A[i] = (seq_t)28 * sum - (seq_t)406 * A[i - 49] + p;
            }
        }
    }
    return A[n];
}
