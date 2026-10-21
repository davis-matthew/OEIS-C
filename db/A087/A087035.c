/*
 * A087035  Maximum value taken on by f(P) = Sum_{i=1..n} p(i)*p(n+1-i) as
 *          {p(1),p(2),...,p(n)} ranges over all permutations P of
 *          {1,2,3,...,n}.
 * https://oeis.org/A087035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*(n-1)*(n+1)*(2*n+3) + ((n%2==0)?6:12)) / 12 ; a(0) :=
 *            0
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* promote index to sequence type for calculations */
    seq_t N = (seq_t)n;

    /* compute the main cubic term: 2*(n-1)*(n+1)*(2*n+3) */
    seq_t term1 = N - 1;            /* n-1   */
    seq_t term2 = N + 1;            /* n+1   */
    seq_t term3 = 2 * N + 3;        /* 2n+3  */
    seq_t prod  = term1 * term2;    /* (n-1)(n+1) */
    prod        = prod * term3;     /* (n-1)(n+1)(2n+3) */
    prod        = prod * 2;         /* 2*(n-1)(n+1)(2n+3) */

    /* add the small parity-dependent constant: 6 if n even, 12 if odd */
    seq_t add = ((n & 1) == 0) ? 6 : 12;

    /* numerator is now divisible by 12 */
    seq_t numer = prod + add;

    return numer / 12;
}
