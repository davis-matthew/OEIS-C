/*
 * A077597  Coefficient of x in the n-th Moebius polynomial (A074586), M(n,x),
 *          which satisfies M(n,-1)=mu(n) the Moebius function of n.
 * https://oeis.org/A077597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor((n+1)/k)
 * Range:     u8 n <= 58, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalent to A006218(n+1) - 1) */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t N = (seq_t)n + 1;           /* n+1 converted once */
    for (idx_t k = 1; k <= n; ++k)
        sum += N / (seq_t)k;          /* floor division using seq_t arithmetic */
    return sum;
}
