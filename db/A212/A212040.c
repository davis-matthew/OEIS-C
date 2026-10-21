/*
 * A212040  Number of lower triangular n X n arrays colored with integers 0
 *          upwards introduced in row major order, with no element equal to
 *          any element within a city block distance of two, and containing
 *          the value n(n+1)/2-3.
 * https://oeis.org/A212040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A212040
    Number of lower triangular n×n arrays colored with integers 0 upwards
    introduced in row major order, with no element equal to any element
    within a city block distance of two, and containing the value
    n(n+1)/2-3.

    Empirical quartic octic closed form (valid for n>1):
    a(n) = (3*n^8 + 12*n^7 - 154*n^6 - 72*n^5 + 2931*n^4
           - 7540*n^3 + 4516*n^2 + 5680*n - 5760) / 384
    with a(1) = 0.  The division by 384 is exact for all integer n.
*/

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the offset (a(1)=0).  For n<=1 we return 0. */
    if (n <= 1)
        return (seq_t)0;

    seq_t N = (seq_t)n;           /* cast index to the sequence type */

    /* Horner's method for the 8th‑degree numerator: */
    seq_t res = (seq_t)3;
    res = res * N + (seq_t)12;
    res = res * N - (seq_t)154;
    res = res * N - (seq_t)72;
    res = res * N + (seq_t)2931;
    res = res * N - (seq_t)7540;
    res = res * N + (seq_t)4516;
    res = res * N + (seq_t)5680;
    res = res * N - (seq_t)5760;

    /* Exact division by 384. */
    return res / (seq_t)384;
}
