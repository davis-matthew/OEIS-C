/*
 * A052574  Expansion of e.g.f. (1-2x)/(1-3x+x^2).
 * https://oeis.org/A052574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A052574 – Expansion of e.g.f. (1-2x)/(1-3x+x^2)
   Recurrence (for n ≥ 0):
     a(0) = 1, a(1) = 1
     a(n+2) = (6 + 3*n) * a(n+1) - (n*n + 3*n + 2) * a(n)
   The recurrence is derived from the e.g.f. and generates all terms
   exactly: a(n) = n! * Σ_{k=1..n} binomial(n-1, k-1) * Fibonacci(k).   */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t a0 = (seq_t)1;   /* a(n)      */
    seq_t a1 = (seq_t)1;   /* a(n+1)    */

    for (idx_t i = 0; i + 2 <= n; ++i)
    {
        /* coefficients for this step : (6+3*i) and (i*i+3*i+2) */
        seq_t coef1 = (seq_t)(6 + 3 * i);
        seq_t coef2 = (seq_t)(i * i + 3 * i + 2);

        seq_t next = coef1 * a1 - coef2 * a0;   /* a(n+2) */

        a0 = a1;
        a1 = next;
    }
    return a1;   /* a(n) is now stored in a1 */
}
