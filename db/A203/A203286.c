/*
 * A203286  Number of arrays of 2n nondecreasing integers in -3..3 with sum
 *          zero and equal numbers greater than zero and less than zero.
 * https://oeis.org/A203286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n^4 + 16*n^3 + 52*n^2 + 80*n + (n is even?48:42)) / 48
 *            This is the explicit polynomial (depending on parity) given in
 *            the OEIS entry for A203286
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t for arithmetic */
    seq_t sn = (seq_t)n;
    seq_t sn2 = sn * sn;          /* n^2 */
    seq_t sn3 = sn2 * sn;         /* n^3 */
    seq_t sn4 = sn3 * sn;         /* n^4 */

    seq_t numer = 2 * sn4 +                /* 2*n^4 */
                  16 * sn3 +               /* 16*n^3 */
                  52 * sn2 +               /* 52*n^2 */
                  80 * sn +                /* 80*n */
                  ((n & 1) ? (seq_t)42 : (seq_t)48);   /* parity term */

    return numer / (seq_t)48;         /* exact integer division */
}
