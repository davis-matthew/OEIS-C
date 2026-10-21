/*
 * A217766  Numerators for a rational approximation to Euler constant.
 * https://oeis.org/A217766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A217766: Numerators for a rational approximation to Euler constant
   Recurrence (valid for n >= 2):
        (16*n - 15)*a(n+1) = (128*n^3 + 40*n^2 - 82*n - 45)*a(n)
                           - n^2*(256*n^3 -240*n^2 +64*n-7)*a(n-1)
                           + (16*n + 1)*n^2*(n - 1)^2*a(n-2)
   Initial values:
        a(0) = 0
        a(1) = 2
        a(2) = 31
   The function below computes a(n) exactly for all n >= 0
   using only the operations allowed on seq_t and idx_t. */

#include <stdint.h>   /* for uint64_t (if needed) */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0u)
        return (seq_t)0;
    if (n == 1u)
        return (seq_t)2;
    if (n == 2u)
        return (seq_t)31;

    /* Variables holding the last three terms: a(i-2), a(i-1), a(i) */
    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)2;   /* a(1) */
    seq_t cur   = (seq_t)31;  /* a(2) */

    /* Iterate to compute terms up to a(n) */
    for (idx_t i = 2u; i <= n-1u; ++i)   /* i serves as n in the recurrence */
    {
        /* Convert loop index to seq_t for arithmetic */
        seq_t n_seq = (seq_t)i;

        /* Pre‑compute powers of n_seq */
        seq_t n2   = n_seq * n_seq;
        seq_t n3   = n2 * n_seq;

        /* Coefficients of the recurrence */
        seq_t coeff1 = 128 * n3 + 40 * n2 - 82 * n_seq - 45;
        seq_t inner  = 256 * n3 - 240 * n2 + 64 * n_seq - 7;
        seq_t coeff2 = n2 * inner;
        seq_t n_minus_1 = n_seq - (seq_t)1;
        seq_t coeff3 = (16 * n_seq + 1) * n2 * (n_minus_1 * n_minus_1);
        seq_t denom  = 16 * n_seq - 15;

        /* Compute numerator: coeff1*a(i) - coeff2*a(i-1) + coeff3*a(i-2) */
        seq_t term1 = coeff1 * cur;
        seq_t term2 = coeff2 * prev1;
        seq_t term3 = coeff3 * prev2;
        seq_t next  = (term1 - term2 + term3) / denom;

        /* Shift window: a(i-1)=cur, a(i)=next, a(i+1)=? */
        prev2 = prev1;
        prev1 = cur;
        cur   = next;
    }

    /* After the loop, cur holds a(n) */
    return cur;
}
