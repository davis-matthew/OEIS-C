/*
 * A135746  E.g.f.: A(x) = Sum_{n>=0} exp(n^2*x) * x^n/n!.
 * https://oeis.org/A135746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)*(k^2)^(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes base^exp for arbitrary‑precision seq_t (or builtin integer type).
   Uses binary exponentiation. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

/* Computes the n-th term of OEIS A135746. */
seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t binom  = 1;   /* C(n,0) */
    idx_t i;

    for (i = 0; i <= n; ++i)
    {
        seq_t k = (seq_t)i;
        seq_t base = k * k;                                     /* k²   */
        seq_t powv  = pow_seq(base, n - i);                     /* (k²)^(n−i) */
        seq_t term  = binom * powv;                             /* C(n,i)*(k²)^(n−i) */
        result += term;

        if (i < n)                                               /* update binom to C(n,i+1) */
            binom = binom * (seq_t)(n - i) / (seq_t)(i + 1);
    }
    return result;
}
