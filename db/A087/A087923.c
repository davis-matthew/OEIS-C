/*
 * A087923  Number of ways of arranging the numbers 1 ... 2n into a 2 X n
 *          array so there is exactly one local maximum.
 * https://oeis.org/A087923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1)*(n-1)! * sum_{k=0..n-1} ( n*binom(2*(n-1),2k) +
 *            binom(2*n-1,2k) ) / binom(n-1,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* factorials 0! … (n-1)! */
    seq_t *fact = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    seq_t sum = (seq_t)0;
    seq_t A = (seq_t)1;             /* binom(2*(n-1), 0) */
    seq_t B = (seq_t)1;             /* binom(2*n-1, 0)   */

    for (idx_t k = 0; k < n; ++k) {
        /* T_k = n*A + B */
        seq_t T = (seq_t)n * A + B;
        /* term_k = T_k * fact[k] * fact[n-1-k]   */
        seq_t term = T * fact[k] * fact[n - 1 - k];
        sum += term;

        if (k + 1 < n) {
            /* Update A:   C(2*(n-1), 2(k+1)) from C(2*(n-1), 2k)  */
            seq_t numA = (seq_t)(2 * n - 2 - 2 * k) *
                          (seq_t)(2 * n - 3 - 2 * k);
            seq_t denA = (seq_t)(2 * k + 1) *
                          (seq_t)(2 * k + 2);
            A = (A * numA) / denA;

            /* Update B:   C(2*n-1, 2(k+1)) from C(2*n-1, 2k)      */
            seq_t numB = (seq_t)(2 * n - 1 - 2 * k) *
                          (seq_t)(2 * n - 2 - 2 * k);
            seq_t denB = denA;               /* same denominator as above */
            B = (B * numB) / denB;
        }
    }

    /* multiply by 2^(n-1) */
    seq_t result = sum;
    for (idx_t i = 1; i < n; ++i)
        result *= (seq_t)2;

    free(fact);
    return result;
}
