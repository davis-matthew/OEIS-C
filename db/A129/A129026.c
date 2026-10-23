/*
 * A129026  a(n) = (1/2)*(n^4 + 11*n^3 + 53*n^2 + 97*n + 54).
 * https://oeis.org/A129026
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(n^4 + 11*n^3 + 53*n^2 + 97*n + 54)
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 300, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*   offset: a(-1) = 0 */
seq_t a(idx_t n)
{
    /* Handle the offset term */
    if (n == -1)
        return (seq_t)0;

    /* n is now non‑negative; cast once */
    seq_t nn = (seq_t)n;

    /* Compute the polynomial terms */
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */

    seq_t numerator =
        n4                         /* n^4   */
        + (seq_t)11 * n3           /* 11*n^3*/
        + (seq_t)53 * n2           /* 53*n^2*/
        + (seq_t)97 * nn           /* 97*n  */
        + (seq_t)54;               /* +54   */

    return numerator / 2;        /* exact division */
}
