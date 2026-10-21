/*
 * A223615  Number of n X 3 0..1 arrays with rows and columns unimodal.
 * https://oeis.org/A223615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (23/360)*n^6 + (31/120)*n^5 + (8/9)*n^4 * + (31/24)*n^3 +
 *            (737/360)*n^2 + (29/20)*n + 1 * * written in integer form: *
 *            a(n) = [ 23*n^6 + 93*n^5 + 320*n^4 + 465*n^3 * + 737*n^2 + 522*n
 *            + 360 ] / 360 * * The sequence is indexed from n = 1 (i.e. a(1)
 *            = 7)
 * Range:     u8 none, u16 n <= 2, u32 n <= 23, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, ptrdiff_t */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is not defined; return 0 safely. */
    if (n == 0) return (seq_t)0;

    seq_t sn   = (seq_t)n;              /* n in seq_t form */
    seq_t n2   = sn * sn;               /* n^2 */
    seq_t n3   = n2 * sn;               /* n^3 */
    seq_t n4   = n3 * sn;               /* n^4 */
    seq_t n5   = n4 * sn;               /* n^5 */
    seq_t n6   = n5 * sn;               /* n^6 */

    /* Build the numerator of the rational expression. */
    seq_t numerator =
        (seq_t)23 * n6          /* 23*n^6   */
        + (seq_t)93 * n5        /* 93*n^5   */
        + (seq_t)320 * n4       /* 320*n^4  */
        + (seq_t)465 * n3       /* 465*n^3  */
        + (seq_t)737 * n2       /* 737*n^2  */
        + (seq_t)522 * sn       /* 522*n    */
        + (seq_t)360;           /* constant term (360 = 1*360) */

    /* Denominator is 360, division is exact. */
    return numerator / (seq_t)360;
}
