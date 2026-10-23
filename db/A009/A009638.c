/*
 * A009638  Expansion of e.g.f.: tan(log(1+sinh(x))).
 * https://oeis.org/A009638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A009638: Expansion of e.g.f.: tan(log(1+sinh(x))).
   Sequence relation: a(0)=0; for n>=0,
   a(n+1) = sum_{k=0}^{n} binom(n,k) * H_k * S_{n-k},
   where H_0=1, H_{m>0} = sum_{i=0}^{m} binom(m,i)*a_i*a_{m-i}
         S_0=1, and S_n follows from
   cosh(x) = S(x)*(1+sinh(x))  in exponential generating functions.
   The implementation uses only integer arithmetic on seq_t.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t N = n;                 /* need coefficients up to index n */
    /* Allocate working arrays */
    seq_t *A = (seq_t *)malloc((N+1) * sizeof(seq_t));   /* a array */
    seq_t *H = (seq_t *)malloc((N+1) * sizeof(seq_t));   /* H array */
    seq_t *S = (seq_t *)malloc((N+1) * sizeof(seq_t));   /* S array */

    /* Init for S computation (division by 1+sinh) */
    S[0] = 1;                     /* p0 - sum = 1 */
    for (idx_t i = 1; i <= N; ++i) {
        seq_t binom = 1;          /* binom(i,0) */
        seq_t sum   = 0;
        /* compute sum_{k=0}^{i-1} binom(i,k) * S_k * q_{i-k} ,
           where q_{j} = 1 if j odd, else 0 */
        for (idx_t k = 0; k < i; ++k) {
            if ( ((i - k) & 1) ) /* q_{i-k} == 1 when (i-k) odd */
                sum += binom * S[k];
            /* next binomial coefficient */
            binom = binom * (seq_t)(i - k) / (seq_t)(k + 1);
        }
        /* p_i is 1 when i even, else 0 */
        seq_t p = ((i & 1) == 0) ? 1 : 0;
        S[i] = p - sum;
    }

    /* Initialize a and H */
    A[0] = 0;
    H[0] = 1;

    for (idx_t m = 0; m < N; ++m) {
        /* Compute a[m+1] via convolution of H and S */
        seq_t r = 0;
        seq_t binom = 1;          /* binom(m,0) */
        for (idx_t k = 0; k <= m; ++k) {
            if (H[k] && S[m - k])    /* skip zero factors */
                r += binom * H[k] * S[m - k];
            binom = binom * (seq_t)(m - k) / (seq_t)(k + 1);
        }
        A[m + 1] = r;

        /* Compute H[m+1] = sum_{i=0}^{m+1} binom(m+1,i) * A[i]*A[m+1-i] */
        if (m + 1 <= N) {
            seq_t sumH = 0;
            seq_t binH = 1;
            idx_t next = m + 1;
            for (idx_t i = 0; i <= next; ++i) {
                if (A[i] && A[next - i])
                    sumH += binH * A[i] * A[next - i];
                binH = binH * (seq_t)(next - i) / (seq_t)(i + 1);
            }
            H[next] = sumH;
        }
    }

    seq_t result = A[N];
    /* Clean up */
    free(A);
    free(H);
    free(S);
    return result;
}
