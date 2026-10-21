/*
 * A084684  Degrees of certain maps (see Comments and Formulas for more
 *          precise definitions).
 * https://oeis.org/A084684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n^2 + 9 - (-1)^n)/8
 * Range:     u8 n <= 6, u16 n <= 294, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;      /* integer value of n in seq_t */
    seq_t num = (seq_t)6 * nn * nn;  /* 6*n^2 */

    /* add 8 if n is even, 10 if n is odd       */
    /* (9 - (-1)^n) equals 8 for even n, 10 for odd n */
    if ((n & 1ULL) == 0)  /* even */
        num += (seq_t)8;
    else                  /* odd */
        num += (seq_t)10;

    return num / (seq_t)8;
}
