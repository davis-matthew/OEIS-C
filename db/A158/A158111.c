/*
 * A158111  E.g.f.: sm^-1(x) = Sum_{n>=0} a(n)*x^(3n+1)/(3n+1)!; a(n) =
 *          coefficient of x^(3n+1)/(3n+1)! in the Maclaurin expansion of the
 *          inverse of the Dixon elliptic function sm(x,0).
 * https://oeis.org/A158111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} (3k-2)*(3k-1)^2 with a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the offset */
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        /* compute 3*i, 3*i-2, 3*i-1 as seq_t values */
        seq_t threei = (seq_t)3 * (seq_t)i;
        seq_t a_val  = threei - (seq_t)2;
        seq_t b_val  = threei - (seq_t)1;

        /* multiply in the required factors */
        result *= a_val;
        result *= b_val;
        result *= b_val;
    }

    return result;
}
