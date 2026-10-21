/*
 * A210982  Zero together with A126264 and positive terms of A051624
 *          interleaved.
 * https://oeis.org/A210982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 227, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (10*n^2 +8*n -3 +(4*n+3)*(-1)^n)/8
 * Derived from the OEIS entry for A210982.
 */
seq_t a(idx_t n)
{
    /* cast the index to the result type to avoid overflow in intermediate steps   */
    seq_t sn = (seq_t)n;
    seq_t sn2 = sn * sn;   /* n^2 in seq_t */

    seq_t numerator;
    if ((n & 1ULL) == 0)          /* n is even */
    {
        /* a(n) = (10*n^2 + 12*n)/8 = n*(5*n+6)/4, but we compute the numerator directly */
        numerator = (seq_t)10 * sn2 + (seq_t)12 * sn;
    }
    else                          /* n is odd */
    {
        /* a(n) = (10*n^2 + 4*n - 6)/8 = (5*n^2 + 2*n - 3)/4, but use the original form     */
        numerator = (seq_t)10 * sn2 + (seq_t)4 * sn - (seq_t)6;
    }

    return numerator / (seq_t)8;
}
