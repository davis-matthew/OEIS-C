/*
 * A119619  a(n) = Product_{i=1..n} i / gcd(i,n).
 * https://oeis.org/A119619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n} i / gcd(i,n) (offset 1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Return 1 for n==0 for safety. */
    if (n == 0U) {
        return (seq_t)1;
    }

    seq_t res = (seq_t)1;
    seq_t sn = (seq_t)n;

    /* Multiply the quotient i / gcd(i,n) for i = 1..n.  Each
       quotient is an integer because gcd(i,n) divides i. */
    for (idx_t i = 1U; i <= n; ++i) {
        seq_t gi = gcd((seq_t)i, sn);
        seq_t q  = (seq_t)i / gi;
        res *= q;
    }
    return res;
}
