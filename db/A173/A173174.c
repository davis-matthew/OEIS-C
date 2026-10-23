/*
 * A173174  a(n) = cosh(2*n*arcsinh(sqrt(n))).
 * https://oeis.org/A173174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n,2*k)*(n+1)^(n-k)*n^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;  /* a(0) = 1 */

    /* (n+1)^n */
    seq_t pow_n1 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow_n1 *= (seq_t)(n + 1);

    seq_t sum  = 0;  /* accumulator for the sum */
    seq_t bin  = 1;  /* binomial coefficient C(2n,2k) */
    seq_t pow_n = 1; /* n^k */

    for (idx_t k = 0; k <= n; ++k) {
        /* Add the k‑th term */
        seq_t term = bin;
        term *= pow_n1;
        term *= pow_n;
        sum += term;

        if (k == n)
            break;  /* last iteration */

        /* Update binomial coefficient to C(2n,2(k+1))
           bin_{k+1} = bin_k * (2n-2k)(2n-2k-1) / ((2k+1)(2k+2))  */
        seq_t num1 = (seq_t)(2 * n - 2 * k);
        seq_t num2 = (seq_t)(2 * n - 2 * k - 1);
        seq_t den1 = (seq_t)(2 * k + 1);
        seq_t den2 = (seq_t)(2 * k + 2);

        bin *= num1;
        bin /= den1;   /* division is exact */
        bin *= num2;
        bin /= den2;   /* division is exact */

        /* Update the powers of n and (n+1) for the next term */
        pow_n1 /= (seq_t)(n + 1);  /* (n+1)^(n-(k+1)) */
        pow_n  *= (seq_t)n;        /* n^(k+1) */
    }

    return sum;
}
