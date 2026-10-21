/*
 * A095822  Numerators of certain upper bounds for Euler's number e.
 * https://oeis.org/A095822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of r(n) = sum_{k=0}^n 1/k! + 1/(n*n!) in lowest
 *            terms. The recurrence uses T_0 = 1 and T_k = k*T_{k-1} + 1,
 *            giving r(n) = ((n+1)*T_n - n*T_{n-1}) / (n*n!)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"          /* provides seq_t gcd(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    if (n < 1)          /* Undefined for n < 1; return 0 as safe value. */
        return (seq_t)0;

    seq_t fact = 1;          /* n! during the loop */
    seq_t T_prev = 1;        /* T_0 */
    seq_t N = 0;             /* Will hold numerator for final n */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t i_seq = (seq_t)i;
        seq_t T = i_seq * T_prev + 1;   /* T_i = i*T_{i-1} + 1 */

        fact *= i_seq;                  /* fact = i!  (after iteration i) */

        if (i == n)                     /* compute numerator for current n */
            N = (i_seq + 1) * T - i_seq * T_prev;

        T_prev = T;
    }

    seq_t denom = (seq_t)n * fact;      /* denom = n*n! */
    seq_t g = gcd(N, denom);            /* reduce fraction */
    return N / g;
}
