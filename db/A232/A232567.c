/*
 * A232567  Number of non-equivalent binary n X n matrices with two
 *          nonadjacent 1's.
 * https://oeis.org/A232567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 2*n^2 - 4*n)/16 if n even; a(n) = (n^4 + 4*n^2 -
 *            8*n + 3)/16 if n odd
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 347, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t N = (seq_t)n;

    /* Compute n^2 and n^4 */
    seq_t N2 = N * N;
    seq_t N4 = N2 * N2;

    seq_t numerator;

    if ((n & 1) == 0) {           /* n is even */
        numerator = N4 + (seq_t)2 * N2 - (seq_t)4 * N;
    } else {                      /* n is odd */
        numerator = N4 + (seq_t)4 * N2 - (seq_t)8 * N + (seq_t)3;
    }

    /* Division by 16 gives the integer value */
    return numerator / (seq_t)16;
}
