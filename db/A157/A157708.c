/*
 * A157708  The z^2 coefficients of the polynomials in the GF4 denominators of
 *          A156933.
 * https://oeis.org/A157708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)/2 (no, this is not the sequence) The actual
 *            A157708 is given by the rational polynomial a(n) = 1/2*n^6 +
 *            5/2*n^5 + 41/8*n^4 + 67/12*n^3 + 27/8*n^2 + 11/12*n which can be
 *            rewritten as an integer division: a(n) = (12*n^6 + 60*n^5 +
 *            123*n^4 + 134*n^3 + 81*n^2 + 22*n)/24 so the computa...
 * Range:     u8 none, u16 n <= 3, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The function signature required by the build system. */
seq_t a(idx_t n)
{
    /* Offset is 1: a(1) is the first term.  For n == 0 return 0. */
    if (n == 0)
        return 0;

    /* Promote n to seq_t to allow arbitrary-precision arithmetic. */
    seq_t nn = (seq_t)n;

    /* Compute the numerator of the rational expression.  Each term
       is added sequentially to avoid excess intermediate size. */
    seq_t num = 12 * nn * nn * nn * nn * nn * nn;   /* 12*n^6 */
    num += 60 * nn * nn * nn * nn * nn;             /* +60*n^5 */
    num += 123 * nn * nn * nn * nn;                /* +123*n^4 */
    num += 134 * nn * nn * nn;                     /* +134*n^3 */
    num += 81  * nn * nn;                          /* +81*n^2  */
    num += 22  * nn;                               /* +22*n    */

    /* Division by 24 is exact for all n. */
    return num / 24;
}
