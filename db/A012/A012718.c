/*
 * A012718  cosh(arctanh(x)*exp(x))=1+1/2!*x^2+6/3!*x^3+33/4!*x^4+180/5!*x^5...
 * https://oeis.org/A012718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in cosh(arctanh(x)*exp(x)) multiplied
 *            by n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(0)=1, a(1)=0, a(2)=1, a(3)=6,... defined in OEIS A012718. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* Allocate arrays for intermediate values */
    idx_t N = n;
    seq_t *a_arr = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *D     = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    seq_t *T     = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!a_arr || !D || !T) return (seq_t)0; /* allocation failure */

    /* 1. Compute the integer coefficients a_k of the series
       A(x) = arctanh(x)*exp(x) :
       a_k = sum_{odd j <= k} P(k,j) / j   where P(k,j) = k!/(k-j)!  */
    a_arr[0] = 0;
    for (idx_t i = 1; i <= N; ++i) {
        seq_t sum = 0;
        seq_t prod = 1;                       /* prod = P(i,j) = i!/(i-j)!   */
        for (idx_t j = 1; j <= i; ++j) {
            prod *= (seq_t)(i - j + 1);       /* multiply next factor          */
            if (j & 1) {                      /* j is odd                     */
                sum += prod / (seq_t)j;       /* division is exact             */
            }
        }
        a_arr[i] = sum;
    }

    /* 2. Recurrence for D[n] = P_n + Q_n and T[n] = P_n - Q_n
       where P_n = n! * [x^n] exp(A(x)) ,
             Q_n = n! * [x^n] exp(-A(x))    */
    D[0] = 2;        /* P_0 = Q_0 = 1  ->  D_0 = 2 */
    T[0] = 0;        /* P_0 - Q_0 = 0 */

    for (idx_t i = 1; i <= N; ++i) {
        seq_t binom = 1;          /* binom = C(i-1, k-1)  (k starts at 1) */
        seq_t Dsum = 0;
        seq_t Tsum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = a_arr[k];
            term *= binom;                      /* a_k * C(i-1, k-1)   */
            Dsum += term * T[i - k];
            Tsum += term * D[i - k];

            /* update binomial coefficient for next k:
               C(i-1, k) = C(i-1, k-1) * (i-k) / k        */
            if (k < i) {
                binom *= (seq_t)(i - k);
                binom /= (seq_t)(k);
            }
        }
        D[i] = Dsum;
        T[i] = Tsum;
    }

    seq_t result = D[N] / (seq_t)2;   /* a(n) = D[n]/2  (D[n] is even) */

    free(a_arr);
    free(D);
    free(T);

    return result;
}
