/*
 * A157053  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 8.
 * https://oeis.org/A157053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2+n+6)*(n^4 + 2*n^3 + 23*n^2 + 22*n + 24)/576
 * Range:     u8 none, u16 n <= 7, u32 n <= 34, u64 n <= 566, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* defined for safety */

    seq_t ni   = (seq_t)n;               /* cast index to seq_t */
    seq_t prod = (seq_t)1;
    seq_t denom = (seq_t)576;

    /* individual factors of the formula */
    seq_t f1 = ni;                       /* n                          */
    seq_t f2 = ni + 1;                   /* n + 1                      */
    seq_t f3 = ni * ni + ni + 6;         /* n^2 + n + 6                */

    /* compute high powers of n */
    seq_t n2 = ni * ni;
    seq_t n3 = n2 * ni;
    seq_t n4 = n3 * ni;

    seq_t f4 = n4 + 2 * n3 + 23 * n2 + 22 * ni + 24;
                                               /* n^4 + 2*n^3 + 23*n^2 + 22*n + 24 */

    /* helper: reduce prod by dividing by the gcd with the denominator */
    seq_t factor, g;

    factor = f1;
    g = gcd(denom, factor);
    prod = prod * (factor / g);
    denom = denom / g;

    factor = f2;
    g = gcd(denom, factor);
    prod = prod * (factor / g);
    denom = denom / g;

    factor = f3;
    g = gcd(denom, factor);
    prod = prod * (factor / g);
    denom = denom / g;

    factor = f4;
    g = gcd(denom, factor);
    prod = prod * (factor / g);
    denom = denom / g;

    /* denom should now be 1; prod is the answer */
    return prod;
}
